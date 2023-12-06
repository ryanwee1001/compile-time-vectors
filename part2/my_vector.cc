#include "my_vector.h"

#include <iostream>
#include <vector>
#include <numeric>
#include <ranges>
#include <algorithm>
#include <exception>

// template <VecLike<int, 100> V>
// constexpr void testFunc(const V& v) {
//     std::cout << "Hello, world!\n";
// }

template <typename T, std::size_t N>
constexpr MyVec<T, N> populate() {
    MyVec<T, N> v(1);
    const int b = 3;
    v.push_back(1);
    v.push_back(2);
    v.push_back(b);
    v.push_back(4);
    v.insert(v.begin()+3, 10);
    return v;
}

template <typename T, std::size_t N>
constexpr T accumulate(MyVec<T, N> v) {
    for (auto it = v.begin(); it != v.end(); ++it)
        std::cout << *it << '\n';
    return std::accumulate(v.begin(), v.end(), static_cast<T>(0));
}

int main() {
    // Testing MyVec
    MyVec<int, 10> v = populate<int, 10>();
    // static_assert(accumulate(v) == 4);
    std::cout << accumulate(v) << '\n';
    std::cout << v.size() << '\n';

    // Testing std::vector: fail
    // error: constexpr variable 'vec' must be initialized by a constant expression
    // pointer to subobject of heap-allocated object is not a constant expression
    // constexpr std::vector<int> vec = populate_std();
    // for (int a : vec)
    //     std::cout << a << '\n';
}
