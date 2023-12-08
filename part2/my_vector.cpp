#include "my_vector.h"
#include "vec_concepts.h"

#include <iostream>
#include <vector>
#include <numeric>
#include <ranges>
#include <algorithm>
#include <exception>
#include <cassert>

template<Vector Vec>
constexpr Vec populate() {
    Vec v;
    v.push_back(1);
    v.push_back(1);
    v.push_back(1);
    v.push_back(1);
    return v;
}

template<Vector Vec>
constexpr Vec prefix_sum(Vec v){
    for (int i = 1; i < v.size(); ++i)
        v[i] += v[i - 1];
    return v;
}

int main() {
    // Example 1: MyVec returning a single int.
    using CVector = MyVec<int, 100>;
    constexpr CVector v1 = populate<CVector>(); // {1,1,1,1}
    static_assert(std::accumulate(v1.begin(), v1.end(), 0) == 4);


    // Example 2: constexpr MyVec returning another constexpr MyVec, 
    // which can be used during runtime.
    constexpr CVector v2 = populate<CVector>(); // {1,1,1,1}
    constexpr CVector v2_res = prefix_sum(v2);
    static_assert(v2_res == CVector({1, 2, 3, 4}));

    // Use during runtime
    // Caveat: Cannot modify CVector since it is const
    std::cout << "Output of Example 2" << '\n';
    for (int a : v2_res)
        std::cout << a << " ";
    std::cout << '\n';

    
    // Example 3: Using std::vector<int> in place of constexpr MyVec in Example 2.
    // Uses the same "prefix_sum" function, except in runtime.
    std::vector<int> v3 = {1,1,1,1};
    std::vector<int> v3_res = prefix_sum(v3);
    assert(v3_res == std::vector<int>({1,2,3,4,}));

    std::cout << "Output of Example 3" << '\n';
    for (int a : v3_res)
        std::cout << a << " ";
    std::cout << '\n';
}
