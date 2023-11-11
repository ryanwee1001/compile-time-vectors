#ifndef MY_VECTOR_H_
#define MY_VECTOR_H_

#include <concepts>
#include <type_traits>
#include <vector>

template <typename T>
class MyVec {
public:
    constexpr MyVec();
    constexpr MyVec(size_t sz);
    // constexpr MyVec(const std::initializer_list<Elem>& i_list);
    void push_back(T val) const;

private:
    std::array<T, 1000> arr;
    size_t v_sz = 0;
    size_t max_capacity = 1000;

};

template <typename T, typename U>
concept VecLike = std::is_same_v<T, std::vector<U>> || 
        std::is_same_v<T, MyVec<U>>;

template <typename T>
constexpr inline MyVec<T>::MyVec() {
    arr.fill(0);
    v_sz = 0;
}

template <typename T>
constexpr inline MyVec<T>::MyVec(size_t sz) {
    arr.fill(0);
    v_sz = sz;
}

template <typename T>
void inline MyVec<T>::push_back(T val) const {
    arr[v_sz++] = val;
}

#endif  // MY_VECTOR_H_
