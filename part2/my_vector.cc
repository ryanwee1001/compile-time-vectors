#include "my_vector.h"

#include <iostream>
#include <vector>

template <VecLike<int> V>
constexpr void testFunc(const V& v) {
    std::cout << "Hello, world!\n";
}

int main() {
    std::vector<int> v{1, 2, 3, 4, 5};
    testFunc(v);
}
