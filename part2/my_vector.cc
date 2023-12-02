#include "my_vector.h"

#include <iostream>
#include <vector>
#include <numeric>
#include <ranges>
#include <algorithm>

// template <VecLike<int, 100> V>
// constexpr void testFunc(const V& v) {
//     std::cout << "Hello, world!\n";
// }

// consteval int f(std::array<int, 10> v) {
//     return std::accumulate(v.begin(), v.end(), 0);
// }

template <typename T, std::size_t N>
constexpr MyVec<T, N> populate() {
    MyVec<T, N> v{};
    v.emplace_back(1);
    v.emplace_back(1);
    v.emplace_back(1);
    v.emplace_back(1);
    return v;
}

template <typename T, std::size_t N>
constexpr T accumulate(MyVec<T, N> v) {
    return std::accumulate(v.begin(), v.end(), static_cast<T>(0));
}

int main() {
    // // Test 1:
    // constexpr std::array<int, 10> v = {1,2,3,4,5,6,7,8,9,10};
    // static_assert(f(v) == 10);
    constexpr MyVec<int, 10> v = populate<int, 10>();
    static_assert(accumulate(v) == 4);
}
