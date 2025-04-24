#ifndef MY_ARRAY_H
#define MY_ARRAY_H
#pragma once
#include <cstddef>
#include <iterator>
#include <algorithm>
#include <compare>

template<typename T, std::size_t N>
class my_array {
    T elems_[N];

public:
    using value_type             = T;
    using reference              = T&;
    using const_reference        = const T&;
    using iterator               = T*;
    using const_iterator         = const T*;
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    constexpr my_array() = default; 

    constexpr my_array(std::initializer_list<T> il) {
        if (il.size() != N)
            throw std::length_error("my_array: wrong init-list size");
        std::copy(il.begin(), il.end(), elems_);
    }
    iterator begin() noexcept { return elems_; }
    const_iterator begin()  const noexcept { return elems_; }
    const_iterator cbegin() const noexcept { return elems_; }
    iterator end() noexcept { return elems_ + N; }
    const_iterator end()  const noexcept { return elems_ + N; }
    const_iterator cend() const noexcept { return elems_ + N; }
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

    constexpr bool is_empty() const noexcept { return N == 0; }
    constexpr size_type size() const noexcept { return N; }

    reference operator[](size_type i) noexcept { return elems_[i]; }
    const_reference operator[](size_type i) const noexcept { return elems_[i]; }
    reference at(size_type i) {
        if (i >= N) throw std::out_of_range("my_array::at");
        return elems_[i];
    }
    const_reference at(size_type i) const {
        if (i >= N) throw std::out_of_range("my_array::at");
        return elems_[i];
    }
    
    reference front() noexcept { return elems_[0]; }
    const_reference front() const noexcept { return elems_[0]; }
    reference back() noexcept { return elems_[N-1]; }
    const_reference back() const noexcept { return elems_[N-1]; }

    void fill(const T& value) { std::fill(begin(), end(), value); }
    void swap(my_array& other) noexcept { std::swap_ranges(begin(), end(), other.begin()); }

    auto operator<=>(const my_array&) const = default;
};
#endif // MY_ARRAY_H