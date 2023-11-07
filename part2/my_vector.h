#ifndef MY_VECTOR_H_
#define MY_VECTOR_H_

#include <concepts>
#include <type_traits>
#include <vector>

template <typename Elem>
class MyVec {
    // TODO
};

template <typename T, typename U>
concept VecLike = std::is_same_v<T, std::vector<U>> || 
        std::is_same_v<T, MyVec<U>>;

#endif  // MY_VECTOR_H_
