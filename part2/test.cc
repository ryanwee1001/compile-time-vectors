#include "my_vector.h"

#include <iostream>
#include <vector>
#include <numeric>
#include <ranges>
#include <algorithm>
#include <exception>
#include <cassert>

void test_emplace_back_1() {
    MyVec<int, 10> v;
    v.emplace_back(1);
    assert(v[0] == 1);
    assert(v.size() == 1);
}

void test_emplace_back_2() {
    MyVec<int, 10> v;
    for (int i = 0; i < 10; ++i)
        v.emplace_back(1);
    assert(std::accumulate(v.begin(), v.end(), 0) == 10);
    assert(v.size() == 10);
}

void test_push_back_1() {
    MyVec<int, 10> v;
    v.push_back(3);
    assert(v[0] == 3);
    assert(v.size() == 1);
}

void test_constructor_1() {
    MyVec<int, 10> v(9);
    assert(v.size() == 9);
}

void test_constructor_2() {
    try { MyVec<int, 10> v(11); }
    catch(const std::length_error& e) {
        assert(e.what() == std::string("Cannot exceed preset capacity."));
    }
    catch(...) { assert(0); }
}

void test_constructor_3() {
    MyVec<int, 10> v1 = {1,1,1,1};
    MyVec<int, 10> v2(std::move(v1));
    assert(v2.size() == 4);
    assert(std::accumulate(v2.begin(), v2.end(), 0) == 4);
}

void test_constructor_4() {
    MyVec<int, 10> v1 = {1,1,1,1};
    MyVec<int, 10> v2(v1);
    assert(v2.size() == 4);
    assert(std::accumulate(v2.begin(), v2.end(), 0) == 4);
}

void test_constructor_5() {
    MyVec<int, 10> v(5, 2);
    assert(v.size() == 5);
    assert(std::accumulate(v.begin(), v.end(), 0) == 10);
}

void test_constructor_6() {
    MyVec<int, 10> v1(5, 2);
    MyVec<int, 10>& v2 = v1;
    assert(v2.size() == 5);
    assert(std::accumulate(v2.begin(), v2.end(), 0) == 10);
}

void test_iterator_1() {
    MyVec<int, 10> v;
    assert(v.begin() == v.end());
}

void test_insert_1() {
    MyVec<int, 10> v;
    auto it = v.insert(v.begin(), 1);
    assert(v[0] == 1);
    assert(v.size() == 1);
}

void test_insert_2() {
    MyVec<int, 10> v;
    auto it1 = v.insert(v.begin(), 1);
    assert(it1 == v.begin());
    auto it2 = v.insert(v.begin(), 2);
    assert(it2 == v.begin());
    assert(v[0] == 2);
    assert(v.size() == 2);
}

void test_insert_3() {
    MyVec<int, 10> v;
    assert(*v.insert(v.begin(), 10, 1) == 1);
    assert(std::accumulate(v.begin(), v.end(), 0) == 10);
    assert(v.size() == 10);
}

void test_insert_4() {
    MyVec<int, 10> v;
    std::vector<int> ins_v = {1,2,3};
    assert(*v.insert(v.begin(), ins_v.begin(), ins_v.end()) == 1);
    assert(v[0] == 1 && v[1] == 2 && v[2] == 3);
    assert(v.size() == 3);
}

void test_insert_5() {
    MyVec<int, 10> v;
    v.emplace_back(0);
    std::vector<int> ins_v = {1,2,3};
    assert(*v.insert(v.begin(), ins_v.begin(), ins_v.end()) == 1);
    assert(v[0] == 1 && v[1] == 2 && v[2] == 3 && v[3] == 0);
    assert(v.size() == 4);
}

void test_insert_6() {
    MyVec<int, 10> v;
    v.emplace_back(0);
    std::vector<int> ins_v = {1,2,3};
    assert(*v.insert(v.end(), ins_v.begin(), ins_v.end()) == 1);
    assert(v[0] == 0 && v[1] == 1 && v[2] == 2 && v[3] == 3);
    assert(v.size() == 4);
}

void test_insert_7() {
    MyVec<int, 10> v;
    assert(*v.insert(v.begin(), {1,2,3}) == 1);
    assert(v[0] == 1 && v[1] == 2 && v[2] == 3);
    assert(v.size() == 3);
}

void test_insert_8() {
    MyVec<int, 10> v;
    v.emplace_back(0);
    std::vector<int> ins_v = {1,2,3};
    assert(*v.insert(v.begin(), {1,2,3}) == 1);
    assert(v[0] == 1 && v[1] == 2 && v[2] == 3 && v[3] == 0);
    assert(v.size() == 4);
}

void test_insert_9() {
    MyVec<int, 10> v;
    v.emplace_back(0);
    std::vector<int> ins_v = {1,2,3};
    assert(*v.insert(v.end(), {1,2,3}) == 1);
    assert(v[0] == 0 && v[1] == 1 && v[2] == 2 && v[3] == 3);
    assert(v.size() == 4);
}

void test_emplace_1() {
    MyVec<int, 10> v;
    auto it = v.emplace(v.begin(), 3);
    assert(it == v.begin());
    assert(v[0] == 3);
    assert(v.size() == 1);
}

void test_erase_1() {
    MyVec<int, 10> v = {1};
    auto it = v.erase(v.begin());
    assert(it == v.end());
    assert(v.empty());
}

void test_erase_2() {
    MyVec<int, 10> v = {1, 2};
    auto it = v.erase(v.begin());
    assert(it == v.begin());
    assert(v.size() == 1);
    assert(v[0] == 2);
}

void test_erase_3() {
    MyVec<int, 10> v = {1, 2};
    auto it = v.erase(v.begin() + 1);
    assert(it == v.end());
    assert(v.size() == 1);
    assert(v[0] == 1);
}

void test_erase_4() {
    MyVec<int, 10> v = {1, 2, 3, 4};
    auto it = v.erase(v.begin(), v.end());
    assert(it == v.end());
    assert(v.empty());
}

void test_erase_5() {
    MyVec<int, 10> v = {1, 2, 3, 4};
    auto it = v.erase(v.begin(), v.end() - 1);
    assert(it == v.begin());
    assert(v.size() == 1);
    assert(v[0] == 4);
}

void test_erase_6() {
    MyVec<int, 10> v = {1, 2, 3, 4};
    auto it = v.erase(v.begin() + 1, v.end());
    assert(it == v.end());
    assert(v.size() == 1);
    assert(v[0] == 1);
}

void test_erase_7() {
    MyVec<int, 10> v = {1, 2, 3, 4};
    auto it = v.erase(v.begin() + 1, v.end() - 1);
    assert(it == v.begin() + 1);
    assert(v.size() == 2);
    assert(v[0] == 1 && v[1] == 4);
}

void test_resize_1() {
    MyVec<int, 10> v = {1,2,3,4};
    v.resize(0);
    v.resize(3);
    assert(std::accumulate(v.begin(), v.end(), 0) == 0);
    assert(v.size() == 3);
}

void test_resize_2() {
    MyVec<int, 10> v;
    v.resize(5, 1);
    assert(std::accumulate(v.begin(), v.end(), 0) == 5);
    assert(v.size() == 5);
}

void test_swap_1() {
    MyVec<int, 10> v1 = {1,1,1,1};
    MyVec<int, 10> v2 = {2,2,2,2,2};
    assert(std::accumulate(v1.begin(), v1.end(), 0) == 4);
    assert(std::accumulate(v2.begin(), v2.end(), 0) == 10);
    assert(v1.size() == 4);
    assert(v2.size() == 5);
    v1.swap(v2);
    assert(std::accumulate(v1.begin(), v1.end(), 0) == 10);
    assert(std::accumulate(v2.begin(), v2.end(), 0) == 4);
    assert(v1.size() == 5);
    assert(v2.size() == 4);
}

void test_comparator_1() {
    MyVec<int, 10> v1 = {1,1,1,1};
    MyVec<int, 10> v2 = {1,1,1,1};
    assert(v1 == v2);
}

void test_comparator_2() {
    MyVec<int, 10> v1 = {1,1,1,1};
    MyVec<int, 10> v2 = {1,1,1,2};
    assert(v1 != v2);
    assert(v1 < v2);
}

void test_comparator_3() {
    MyVec<int, 10> v1 = {2,1,1,1};
    MyVec<int, 10> v2 = {1,1,1,1};
    assert(v1 != v2);
    assert(v1 > v2);
}

void test_comparator_4() {
    MyVec<int, 10> v1 = {1,1,1,1,1};
    MyVec<int, 10> v2 = {1,1,1,1};
    assert(v1 != v2);
    assert(v1 > v2);
}

constexpr void test_concept_1() {
    static_assert(Container<std::vector<int>>);
    static_assert(Container<MyVec<int, 10>>);
}

constexpr void test_concept_2() {
    static_assert(Vector<std::vector<int>>);
    static_assert(Vector<MyVec<int, 10>>);
}

int main() {
    test_emplace_back_1(); test_emplace_back_2();
    test_push_back_1();
    test_constructor_1(); test_constructor_2(); test_constructor_3(); test_constructor_4(); test_constructor_5(); test_constructor_6();
    test_iterator_1();
    test_insert_1(); test_insert_2(); test_insert_3(); test_insert_4(); test_insert_5(); 
    test_insert_6(); test_insert_7(); test_insert_8(); test_insert_9();
    test_emplace_1();
    test_erase_1(); test_erase_2(); test_erase_3(); test_erase_4(); test_erase_5(); test_erase_6(); test_erase_7();
    test_resize_1(); test_resize_2();
    test_swap_1();
    test_comparator_1(); test_comparator_2(); test_comparator_3(); test_comparator_4();
    std::cout << "All tests passed" << std::endl;
}