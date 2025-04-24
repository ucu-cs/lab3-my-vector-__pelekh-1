#ifndef MY_UNIQUE_PTR_H
#define MY_UNIQUE_PTR_H
#pragma once
#include <utility>
#include <memory>

template<typename T, typename Deleter = std::default_delete<T>>
class my_unique_ptr {
    T* ptr_ = nullptr;
    Deleter del_;

public:
    constexpr my_unique_ptr() noexcept = default;
    explicit my_unique_ptr(T* p) noexcept : ptr_(p) {}
    my_unique_ptr(T* p, Deleter d) noexcept : ptr_(p), del_(std::move(d)) {}

    my_unique_ptr(my_unique_ptr&& other) noexcept
        : ptr_(std::exchange(other.ptr_, nullptr)), del_(std::move(other.del_)) {}
    my_unique_ptr& operator=(my_unique_ptr&& other) noexcept {
        reset(other.release());
        del_ = std::move(other.del_);
        return *this;
    }
    my_unique_ptr(const my_unique_ptr&)            = delete;
    my_unique_ptr& operator=(const my_unique_ptr&) = delete;

    ~my_unique_ptr() { reset(); }

    T*       get()       noexcept { return ptr_; }
    const T* get() const noexcept { return ptr_; }
    T&       operator*()  const noexcept { return *ptr_; }
    T*       operator->() const noexcept { return ptr_; }
    explicit operator bool() const noexcept { return ptr_ != nullptr; }

    T* release() noexcept { return std::exchange(ptr_, nullptr); }
    void reset(T* p = nullptr) noexcept {
        if (ptr_) del_(ptr_);
        ptr_ = p;
    }
    void swap(my_unique_ptr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(del_, other.del_);
    }
};
#endif