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

void test_iterator_1() {
    MyVec<int, 10> v;
    assert(v.begin() == v.end());
}

void test_insert_1() {
    MyVec<int, 10> v;
    v.insert(v.begin(), 1);
    assert(v[0] == 1);
    assert(v.size() == 1);
}

void test_insert_2() {
    MyVec<int, 10> v;
    assert(v.insert(v.begin(), 1) == v.begin());
    assert(v.insert(v.begin(), 2) == v.begin());
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
    assert(v.emplace(v.begin(), 3) == v.begin());
    assert(v[0] == 3);
    assert(v.size() == 1);
}

void test_erase_1() {
    MyVec<int, 10> v = {1,2,3}; // TODO
}

int main() {
    test_emplace_back_1(); test_emplace_back_2();
    test_push_back_1();
    test_constructor_1();
    test_constructor_2();
    test_iterator_1();
    test_insert_1(); test_insert_2(); test_insert_3(); test_insert_4(); test_insert_5(); 
    test_insert_6(); test_insert_7(); test_insert_8(); test_insert_9();
    test_emplace_1();
    std::cout << "All tests passed" << std::endl;
}