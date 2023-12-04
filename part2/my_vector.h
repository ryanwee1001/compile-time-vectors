#ifndef MY_VECTOR_H_
#define MY_VECTOR_H_

#include <iostream>
#include <concepts>
#include <type_traits>
#include <vector>
#include <stdexcept>
#include <utility>
#include <format>
#include <iterator>

template <typename T, std::size_t N>
class MyVec {
public:
    // Member types
    using value_type = T;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = T*;
    using const_iterator = T * const;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // Initializers
    constexpr MyVec() : arr() {}
    constexpr explicit MyVec(size_type count) : arr() {
        if (count > cap)
            throw std::length_error("Cannot exceed preset capacity.");
        sz = count;
    }
    // TODO
    // constexpr MyVec(const std::initializer_list<Elem>& i_list);
    
    // Element access
    constexpr reference at(size_type pos) {
        _check_index(pos);
        return arr[pos];
    }

    constexpr const_reference at(size_type pos) const {
        _check_index(pos);
        return arr[pos];
    }

    constexpr reference operator[](size_type pos) { return arr[pos]; }
    constexpr const_reference operator[](size_type pos) const { return arr[pos]; }
    constexpr reference front() { return arr[0]; }
    constexpr const_reference front() const { return arr[0]; }
    constexpr reference back() { return arr[sz - 1]; }
    constexpr const_reference back() const { return arr[sz - 1]; }
    constexpr const T* data() const noexcept { return arr.data(); }


    // Iterators
    constexpr iterator begin() noexcept { return arr.begin(); }
    constexpr iterator end() noexcept { return arr.begin() + sz; }
    constexpr const_iterator begin() const noexcept { return arr.begin(); }
    constexpr const_iterator end() const noexcept { return arr.begin() + sz; }
    constexpr const_iterator cbegin() const noexcept { return arr.cbegin(); }
    constexpr const_iterator cend() const noexcept { return arr.cbegin() + sz; }
    constexpr reverse_iterator rbegin() noexcept { return arr.rend() - sz; }
    constexpr reverse_iterator rend() noexcept { return arr.rend(); }
    constexpr const_reverse_iterator rbegin() const noexcept { return arr.rend() - sz; }
    constexpr const_reverse_iterator rend() const noexcept { return arr.rend(); }
    constexpr const_reverse_iterator crbegin() const noexcept { return arr.crend() - sz; }
    constexpr const_reverse_iterator crend() const noexcept { return arr.crend(); }

    // Capacity
    [[nodiscard]] constexpr bool empty() const noexcept { return sz == 0; }
    constexpr size_type size() const noexcept { return sz; }
    constexpr size_type max_size() const noexcept { return cap; }
    constexpr size_type capacity() const noexcept { return cap; }

    // Modifiers
    // TODO
    constexpr void clear() noexcept { sz = 0; }
    constexpr iterator insert(const_iterator pos, const T& value) {
        size_type pos_idx = pos - begin();
        _insert_check(pos_idx, 1);
        _shift_n_forward(pos_idx, 1);
        ++sz;
        arr[pos_idx] = value;
        return begin() + pos_idx;
    }

    constexpr iterator insert(const_iterator pos, T&& value) {
        size_type pos_idx = pos - begin();
        _insert_check(pos_idx, 1);
        _shift_n_forward(pos_idx, 1);
        ++sz;
        arr[pos_idx] = value;
        return begin() + pos_idx;
    }

    constexpr iterator insert(const_iterator pos, size_type count, const T& value) {
        if (count == 0) return pos;
        size_type pos_idx = pos - begin();
        _insert_check(pos_idx, count);
        _shift_n_forward(pos_idx, count);
        sz += count;
        for (int i = 0; i < count; ++i)
            arr[pos_idx + i] = value;
        return begin() + pos_idx;
    }

    template<std::input_iterator InputIt>
    constexpr iterator insert(const_iterator pos, InputIt first, InputIt last) {
        if (first == last) return pos;
        size_type pos_idx = pos - begin();
        size_type count = 0;
        for (InputIt it = first; it != last; ++it)
            ++count;
        _insert_check(pos_idx, count);
        _shift_n_forward(pos_idx, count);
        sz += count;
        int i = 0;
        for (InputIt it = first; it != last; ++it, ++i)
            arr[pos_idx + i] = *it;
        return begin() + pos_idx;
    }

    constexpr iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
        if (ilist.size() == 0) return pos;
        size_type pos_idx = pos - begin();
        size_type count = ilist.size();
        _insert_check(pos_idx, count);
        _shift_n_forward(pos_idx, count);
        sz += count;
        int i = 0;
        for (auto it = ilist.begin(); it != ilist.end(); ++it, ++i)
            arr[pos_idx + i] = *it;
        return begin() + pos_idx;
    }

    template<class... Args>
    constexpr iterator emplace(const_iterator pos, Args&&... args) {
        size_type pos_idx = pos - begin();
        _insert_check(pos_idx, 1);
        _shift_n_forward(pos_idx, 1);
        ++sz;
        arr[pos_idx] = T(std::forward<Args>(args)...);
        return begin() + pos_idx;
    }

    // TO TEST
    constexpr iterator erase(const_iterator pos) {
        size_type pos_idx = pos - begin();
        _shift_n_backward(pos_idx, 1);
        --sz;        
        return begin() + pos_idx;
    }

    constexpr iterator erase(const_iterator first, const_iterator last) {
        size_type pos_idx = first - begin();
        size_type count = last - first + 1;
        _shift_n_backward(pos_idx, count);
        sz -= count;
        return begin() + pos_idx;
    }

    constexpr void push_back(const T& val) { emplace_back(val); }
    constexpr void push_back(T&& val) { emplace_back(std::move(val)); }

    template<class... Args>
    constexpr reference emplace_back(Args&&... args) {
        _check_length(sz + 1);
        arr[sz++] = T(std::forward<Args>(args)...);
        return arr[sz];
    }

private:
    std::array<T, N> arr;
    std::size_t sz = 0;
    std::size_t cap = N;

    constexpr void _check_index(size_type pos) {
        if (pos < 0 || pos >= sz) throw std::out_of_range("Index out of range.");
    }

    constexpr void _check_insert_index(size_type pos) {
        if (pos < 0 || pos > sz) throw std::out_of_range("Index out of range.");
    }

    constexpr void _check_length(size_type new_size) {
        if (new_size > cap) throw std::length_error("Cannot exceed preset capacity.");
    }

    constexpr void _shift_n_forward(size_type pos, size_type n) {
        for (int i = int(sz) - 1; i >= int(pos); --i) arr[i + n] = arr[i];
    }

    constexpr void _shift_n_backward(size_type pos, size_type n) {
        for (int i = pos + n; i < sz; ++i) arr[i - n] = arr[i];
    }

    constexpr void _insert_check(size_type pos, size_type count) {
        _check_length(sz + count);
        _check_insert_index(pos);
    }
};

// template <typename T, typename U>
// concept VecLike = std::is_same_v<T, std::vector<U>> || 
//         std::is_same_v<T, MyVec<U>>;

#endif  // MY_VECTOR_H_
