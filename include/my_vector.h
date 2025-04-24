#ifndef MY_VECTOR_H
#define MY_VECTOR_H
#pragma once
#include <memory>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <initializer_list>
#include <utility>
#include <compare>

template<typename T>
class my_vector {
    T*      data_   = nullptr;
    size_t  size_   = 0;
    size_t  cap_    = 0;

    static size_t grow_capacity(size_t old_cap, size_t need) {
        size_t new_cap = old_cap ? old_cap * 2 : 1;
        while (new_cap < need) new_cap *= 2;
        return new_cap;
    }

    void ensure_capacity(size_t need) {
        if (need <= cap_) return;
        reserve(grow_capacity(cap_, need));
    }

public:
    using value_type             = T;
    using reference              = T&;
    using const_reference        = const T&;
    using pointer                = T*;
    using const_pointer          = const T*;
    using iterator               = T*;
    using const_iterator         = const T*;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using difference_type        = std::ptrdiff_t;
    using size_type              = size_t;

    my_vector() noexcept = default;

    explicit my_vector(size_t n, const T& value = T()) { assign(n, value); }

    template<std::input_iterator It>
    my_vector(It first, It last) { assign(first, last); }

    my_vector(std::initializer_list<T> il) { assign(il.begin(), il.end()); }

    my_vector(const my_vector& other) { assign(other.begin(), other.end()); }

    my_vector(my_vector&& other) noexcept
        : data_(std::exchange(other.data_, nullptr)),
          size_(std::exchange(other.size_, 0)),
          cap_(std::exchange(other.cap_, 0)) {}

    ~my_vector() { clear(); ::operator delete[](data_); }

    my_vector& operator=(const my_vector& rhs) {
        if (this != &rhs) assign(rhs.begin(), rhs.end());
        return *this;
    }
    my_vector& operator=(my_vector&& rhs) noexcept {
        if (this != &rhs) {
            clear(); ::operator delete[](data_);
            data_ = std::exchange(rhs.data_, nullptr);
            size_ = std::exchange(rhs.size_, 0);
            cap_  = std::exchange(rhs.cap_, 0);
        }
        return *this;
    }
    my_vector& operator=(std::initializer_list<T> il) {
        assign(il.begin(), il.end());
        return *this;
    }

    reference operator[](size_t i) noexcept { return data_[i]; }
    const_reference operator[](size_t i) const noexcept { return data_[i]; }

    reference at(size_t i) {
        if (i >= size_) throw std::out_of_range("my_vector::at");
        return data_[i];
    }
    const_reference at(size_t i) const {
        if (i >= size_) throw std::out_of_range("my_vector::at");
        return data_[i];
    }
    reference front() noexcept { return data_[0]; }
    const_reference front() const noexcept { return data_[0]; }
    reference back() noexcept { return data_[size_-1]; }
    const_reference back() const noexcept { return data_[size_-1]; }

    iterator begin() noexcept { return data_; }
    const_iterator begin()  const noexcept { return data_; }
    const_iterator cbegin() const noexcept { return data_; }
    iterator end() noexcept { return data_ + size_; }
    const_iterator end() const noexcept { return data_ + size_; }
    const_iterator cend() const noexcept { return data_ + size_; }
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

    bool is_empty() const noexcept { return size_ == 0; }
    size_t size() const noexcept { return size_; }
    size_t capacity() const noexcept { return cap_; }
    void reserve(size_t new_cap) {
        if (new_cap <= cap_) return;
        T* new_data = static_cast<T*>(::operator new[](new_cap * sizeof(T)));
        std::uninitialized_move(data_, data_ + size_, new_data);
        std::destroy_n(data_, size_);
        ::operator delete[](data_);
        data_ = new_data;
        cap_  = new_cap;
    }
    void shrink_to_fit() {
        if (size_ == cap_) return;
        T* new_data = size_ ? static_cast<T*>(::operator new[](size_ * sizeof(T))) : nullptr;
        std::uninitialized_move(data_, data_ + size_, new_data);
        std::destroy_n(data_, size_);
        ::operator delete[](data_);
        data_ = new_data;
        cap_  = size_;
    }

    void clear() noexcept {
        std::destroy_n(data_, size_);
        size_ = 0;
    }
    void swap(my_vector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(cap_,  other.cap_);
    }
    void push_back(const T& value) {
        ensure_capacity(size_ + 1);
        new (data_ + size_) T(value);
        ++size_;
    }
    void push_back(T&& value) {
        ensure_capacity(size_ + 1);
        new (data_ + size_) T(std::move(value));
        ++size_;
    }
    template<typename... Args>
    reference emplace_back(Args&&... args) {
        ensure_capacity(size_ + 1);
        new (data_ + size_) T(std::forward<Args>(args)...);
        return data_[size_++];
    }
    void pop_back() {
        data_[--size_].~T();
    }

    iterator insert(iterator pos, const T& value) {
        return emplace(pos, value);
    }
    iterator insert(iterator pos, T&& value) {
        return emplace(pos, std::move(value));
    }
    template<std::input_iterator It>
    iterator insert(iterator pos, It first, It last) {
        size_t idx = pos - begin();
        size_t count = std::distance(first, last);
        ensure_capacity(size_ + count);
        iterator it_pos = begin() + idx;
        std::move_backward(it_pos, end(), end() + count);
        std::uninitialized_copy(first, last, it_pos);
        size_ += count;
        return it_pos;
    }

    iterator erase(iterator pos) {
        return erase(pos, pos + 1);
    }
    iterator erase(iterator first, iterator last) {
        iterator new_end = std::move(last, end(), first);
        std::destroy(new_end, end());
        size_ -= (last - first);
        return first;
    }

    void resize(size_t n, const T& value = T()) {
        if (n < size_) {
            erase(begin() + n, end());
        } else if (n > size_) {
            ensure_capacity(n);
            std::uninitialized_fill(data_ + size_, data_ + n, value);
            size_ = n;
        }
    }

    // ====== comparisons ======
    auto operator<=>(const my_vector&) const = default;

private:
    template<typename... Args>
    iterator emplace(iterator pos, Args&&... args) {
        size_t idx = pos - begin();
        ensure_capacity(size_ + 1);
        pos = begin() + idx;
        if (pos == end()) {
            new (data_ + size_) T(std::forward<Args>(args)...);
        } else {
            new (data_ + size_) T(std::move(back()));
            std::move_backward(pos, end() - 1, end());
            *pos = T(std::forward<Args>(args)...);
        }
        ++size_;
        return pos;
    }

    template<std::input_iterator It>
    void assign(It first, It last) {
        clear();
        size_t n = std::distance(first, last);
        ensure_capacity(n);
        std::uninitialized_copy(first, last, data_);
        size_ = n;
    }
    void assign(size_t n, const T& value) {
        clear();
        ensure_capacity(n);
        std::uninitialized_fill_n(data_, n, value);
        size_ = n;
    }
};
#endif