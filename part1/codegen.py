"""This file generates C++ source code for summing over a std::vector.

In particular, it generates three files:

1. vec_test_runtime.cpp, which does this computation at runtime
2. vec_test_constexpr.cpp, which does this computation possibly at compile time
3. vec_test_consteval.cpp, which does this computation at compile time

Each file roughly follows the following structure:

    <boilerplate_head>
    <compiletime_body> OR <runtime_body>
    <boilerplate_tail>

We pre-generate this code, because std::random_device is a runtime random
number generator. To write a constexpr or consteval function, we need to bake
all the elements used to populate the std::vector into the source code itself
and explicitly add them to the std::vector one by one.
"""


from random import randint
from typing import List


boilerplate_head = """// This file was auto-generated by `codegen.py`.
// Do not modify it manually unless there is good reason to.

#include <chrono>
#include <concepts>
#include <iostream>
#include <numeric>
#include <string_view>
#include <type_traits>
#include <vector>

// Checks that some functor Func can be invoked at compile time on a std::vector
// containing elements of type T.
//
// Usage of std::bool_constant was inspired by stackoverflow question 63326542.
template <typename Func, typename T>
concept CompileTimeInvocable = requires (Func f) {
    { std::bool_constant<(Func()(std::vector<T>{}), true)>() };
    { Func()(std::vector<T>{}) } -> std::same_as<T>;
};

// Wrapper around the std::is_arithmetic type trait.
template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

// Functor that sums the elements of a vector.
template <Arithmetic T>
struct SimpleSum {
    constexpr T operator()(const std::vector<T>& v) {
        return std::accumulate(v.begin(), v.end(), static_cast<T>(0));
    }
};

// Functor that multiplies the elements of a vector (while taking mod 1e9).
template <Arithmetic T>
struct SimpleProd {
    constexpr T operator()(const std::vector<T>& v) {
        return std::accumulate(v.begin(), v.end(), static_cast<T>(1), 
            [](const T& accum, const T& elem) -> T {
                return (accum * elem) % static_cast<T>(1e9);
            }
        );
    }
};
"""

boilerplate_tail = """
int main() {{
    using namespace std::literals;
    
    int numRuns = {num_runs};
    runTestCase<SimpleSum<long long>, long long>(numRuns, "SimpleSum"sv);
    runTestCase<SimpleProd<long long>, long long>(numRuns, "SimpleProd"sv);
}}
"""

# The code below does the computation in a constexpr or consteval function.
compiletime_contents = """
// Populates a std::vector, then performs some computation over its elements.
// This function is executed {description}.
//
// To obtain statistics on runtime, time how long it takes for a call to this
// function to be completed. We don't do the timing inside this function,
// because reading the current system time can only be done at runtime.
//
// @tparam Func the computation to be executed
// @tparam T the type of elements in the std::vector
template <typename Func, typename T>
requires CompileTimeInvocable<Func, T>
{specifier} T doComputation() {{
    // The repeated statements are ugly, but we need to explicitly add each
    // individual element rather than using a std::random_device and a loop so
    // that the function can be executed at compile time.
    std::vector<T> v;
{populate_vector}

    return Func()(v);
}}

// Wrapper over `doComputation` that:
// (1) executes the test `numRuns` number of times
// (2) times each function call and prints the relevant results
template <typename Func, typename T>
void runTestCase(int numRuns, std::string_view testName) {{
    for (int i = 0; i < numRuns; ++i) {{
        auto start = std::chrono::high_resolution_clock::now();
        T res = doComputation<Func, T>();
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << testName << " result: " << res << "\\n";
        std::cout << testName << " timing: " << (end-start).count() << " ns\\n";
    }}
    std::cout << "\\n";
}}
"""

# The code below does the computation in a runtime function. We populate the
# std::vector using a separate function, so that we don't end up including the
# time taken to populate the std::vector in our measurements.
runtime_contents = """
// Populates a std::vector with the same elements used to populate the
// std::vector in the compile-time code.
//
// The repeated statements are ugly, but we do this instead of using a 
// std::random_device and a loop so that we can populate the std::vector with
// the same elements used in the compile-time code.
template <typename T>
std::vector<T> populateVec() {{
    std::vector<T> v;
{populate_vector}
    return v;
}}

// Performs some computation over the elements of a pre-populated std::vector.
// This function is executed at runtime.
//
// @tparam Func the computation to be executed
// @tparam T the type of elements in the std::vector
template <typename Func, typename T>
requires CompileTimeInvocable<Func, T>
T doComputation(const std::vector<T>& v) {{
    return Func()(v);
}}

// Wrapper over `doComputation` that:
// (1) executes the test `numRuns` number of times
// (2) times each function call and prints the relevant results
template <typename Func, typename T>
void runTestCase(int numRuns, std::string_view testName) {{
    std::vector<T> v = populateVec<T>();
    for (int i = 0; i < numRuns; ++i) {{
        auto start = std::chrono::high_resolution_clock::now();
        T res = doComputation<Func, T>(v);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << testName << " result: " << res << "\\n";
        std::cout << testName << " timing: " << (end-start).count() << " ns\\n";
    }}
    std::cout << "\\n";
}}
"""


def generate_filename(test_type: str, num_elems: int) -> str:
    return f"vectest_{num_elems}_{test_type}.cpp"


def generate_randomints(size: int) -> List[int]:
    l = []
    for _ in range(size):
        l.append(randint(-1e9, 1e9))
    return l


def generate_populationcode(l: List[int]) -> str:
    s = ""
    for elem in l:
        s += f"    v.emplace_back({elem});\n"
    return s


def generate_compiletime_code(
        l: List[int], num_runs: int, test_type: str
) -> None:
    filename = generate_filename(test_type, len(l))
    specifier = test_type + " "
    description = "at compile time" if (
        test_type == "consteval"
    ) else "possibly at compile time"

    filecontents = boilerplate_head
    filecontents += compiletime_contents.format(
        description=description,
        specifier=specifier,
        populate_vector=generate_populationcode(l),
    )
    filecontents += boilerplate_tail.format(num_runs=num_runs)
    with open(filename, "w") as f:
        f.write(filecontents)


def generate_runtime_code(l: List[int], num_runs: int) -> None:
    filename = generate_filename("runtime", len(l))

    filecontents = boilerplate_head
    filecontents += runtime_contents.format(
        populate_vector=generate_populationcode(l),
    )
    filecontents += boilerplate_tail.format(num_runs=num_runs)
    with open(filename, "w") as f:
        f.write(filecontents)


def main() -> None:
    # Each test is run `num_runs` number of times.
    num_runs = 1
    num_elems = 1

    # By crude binary search, it seems that ~7400 is the maximum number of
    # elements the compiler can handle when doing compile-time computation
    # before it runs out of memory.
    l = generate_randomints(num_elems)
    generate_runtime_code(l, num_runs)
    generate_compiletime_code(l, num_runs, "constexpr")
    generate_compiletime_code(l, num_runs, "consteval")


if __name__ == '__main__':
    main()
