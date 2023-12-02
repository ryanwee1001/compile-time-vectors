#ifndef MY_VECTOR_H_
#define MY_VECTOR_H_

#include <concepts>
#include <type_traits>
#include <vector>
#include <stdexcept>
#include <utility>

template <typename T, std::size_t N>
class MyVec {
public:
    using iterator = T*;
    using const_iterator = const T*;
    using value_type = T;
    using size_type = std::size_t;

    constexpr MyVec() : arr() {}
    // constexpr MyVec(size_t sz);
    // constexpr MyVec(const std::initializer_list<Elem>& i_list);
    T constexpr operator[](const unsigned int idx) const { return arr[idx]; }
    iterator constexpr begin() { return arr.begin(); }
    iterator constexpr end() { return arr.end(); }
    const_iterator constexpr begin() const { return arr.begin(); }
    const_iterator constexpr end() const { return arr.end(); }
    
    void constexpr push_back(const T val) { emplace_back(std::move(val)); }

    template<class... Args>
    constexpr T& emplace_back(Args&&... args) {
        if (size == capacity) 
            throw std::length_error("Cannot exceed preset capacity.");
        arr[size++] = T(std::forward<Args>(args)...);
        return arr[size];
    }

private:
    std::array<T, N> arr;
    std::size_t size = 0;
    std::size_t capacity = N;
};

// template <typename T, typename U>
// concept VecLike = std::is_same_v<T, std::vector<U>> || 
//         std::is_same_v<T, MyVec<U>>;

#endif  // MY_VECTOR_H_
