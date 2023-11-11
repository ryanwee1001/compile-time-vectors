#include "my_vector.h"

#include <iostream>
#include <vector>
#include <numeric>
#include <ranges>
#include <algorithm>

template <VecLike<int, 100> V>
constexpr void testFunc(const V& v) {
    std::cout << "Hello, world!\n";
}

int main() {
    // Test 2:
    constexpr MyVec<int> v(10);
    v.push_back(1);
}
