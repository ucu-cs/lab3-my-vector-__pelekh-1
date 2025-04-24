#ifndef MY_SHARED_PTR_H
#define MY_SHARED_PTR_H
#pragma once
#include <atomic>
#include <utility>

namespace detail {
struct control_block_base {
    std::atomic<size_t> strong{1};
    std::atomic<size_t> weak{1};
    virtual void destroy() noexcept = 0;
    virtual ~control_block_base() = default;
};

template<typename T, typename Deleter>
struct control_block : control_block_base {
    T*       ptr;
    Deleter  del;
    control_block(T* p, Deleter d) : ptr(p), del(std::move(d)) {}
    void destroy() noexcept override { del(ptr); }
};
} 

template<typename T>
class my_weak_ptr;                   

template<typename T>
class my_shared_ptr {
    template<typename U> friend class my_shared_ptr;
    template<typename U> friend class my_weak_ptr;

    T*                         ptr_ = nullptr;
    detail::control_block_base* ctrl_ = nullptr;

    void inc() noexcept {
        if (ctrl_) ctrl_->strong.fetch_add(1, std::memory_order_relaxed);
    }
    void dec() noexcept {
        if (!ctrl_) return;
        if (ctrl_->strong.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            ctrl_->destroy();
            if (ctrl_->weak.fetch_sub(1, std::memory_order_acq_rel) == 1)
                delete ctrl_;
        }
    }
public:
    constexpr my_shared_ptr() noexcept = default;

    template<typename Deleter = std::default_delete<T>>
    explicit my_shared_ptr(T* p, Deleter d = Deleter())
        : ptr_(p),
          ctrl_(new detail::control_block<T,Deleter>(p, std::move(d))) {}

    my_shared_ptr(const my_shared_ptr& other) noexcept : ptr_(other.ptr_), ctrl_(other.ctrl_) { inc(); }
    template<typename U>
    my_shared_ptr(const my_shared_ptr<U>& other) noexcept : ptr_(other.ptr_), ctrl_(other.ctrl_) { inc(); }

    my_shared_ptr(my_shared_ptr&& other) noexcept
        : ptr_(std::exchange(other.ptr_, nullptr)), ctrl_(std::exchange(other.ctrl_, nullptr)) {}
    template<typename U>
    my_shared_ptr(my_shared_ptr<U>&& other) noexcept
        : ptr_(std::exchange(other.ptr_, nullptr)), ctrl_(std::exchange(other.ctrl_, nullptr)) {}

    ~my_shared_ptr() { dec(); }

    my_shared_ptr& operator=(const my_shared_ptr& rhs) noexcept {
        if (this != &rhs) { dec(); ptr_=rhs.ptr_; ctrl_=rhs.ctrl_; inc(); }
        return *this;
    }
    my_shared_ptr& operator=(my_shared_ptr&& rhs) noexcept {
        if (this != &rhs) { dec(); ptr_=std::exchange(rhs.ptr_,nullptr); ctrl_=std::exchange(rhs.ctrl_,nullptr); }
        return *this;
    }

    T*       get()  noexcept { return ptr_; }
    const T* get()  const noexcept { return ptr_; }
    T&       operator*()  const noexcept { return *ptr_; }
    T*       operator->() const noexcept { return ptr_; }
    explicit operator bool() const noexcept { return ptr_ != nullptr; }

    size_t use_count() const noexcept { return ctrl_ ? ctrl_->strong.load() : 0; }
    bool   unique()    const noexcept { return use_count() == 1; }

    void reset() noexcept { my_shared_ptr().swap(*this); }
    template<typename Deleter = std::default_delete<T>>
    void reset(T* p, Deleter d = Deleter()) { my_shared_ptr(p,std::move(d)).swap(*this); }

    void swap(my_shared_ptr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(ctrl_, other.ctrl_);
    }
};
#endif
