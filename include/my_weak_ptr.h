#ifndef MY_WEAK_PTR_H
#define MY_WEAK_PTR_H
template<typename T>
class my_weak_ptr {
    template<typename U> friend class my_shared_ptr;

    detail::control_block_base* ctrl_ = nullptr;

    void inc() noexcept {
        if (ctrl_) ctrl_->weak.fetch_add(1, std::memory_order_relaxed);
    }
    void dec() noexcept {
        if (!ctrl_) return;
        if (ctrl_->weak.fetch_sub(1, std::memory_order_acq_rel) == 1)
            if (ctrl_->strong.load() == 0)
                delete ctrl_;
    }
public:
    my_weak_ptr() noexcept = default;
    my_weak_ptr(const my_shared_ptr<T>& sp) noexcept : ctrl_(sp.ctrl_) { inc(); }

    my_weak_ptr(const my_weak_ptr& other) noexcept : ctrl_(other.ctrl_) { inc(); }
    my_weak_ptr(my_weak_ptr&& other) noexcept : ctrl_(std::exchange(other.ctrl_, nullptr)) {}
    ~my_weak_ptr() { dec(); }

    my_weak_ptr& operator=(const my_weak_ptr& rhs) noexcept {
        if (this != &rhs) { dec(); ctrl_=rhs.ctrl_; inc(); }
        return *this;
    }
    my_weak_ptr& operator=(my_weak_ptr&& rhs) noexcept {
        if (this != &rhs) { dec(); ctrl_=std::exchange(rhs.ctrl_, nullptr); }
        return *this;
    }
    my_shared_ptr<T> lock() const noexcept
    {
        if (!ctrl_ || ctrl_->strong.load(std::memory_order_acquire) == 0)
            return {};                                 

        ctrl_->strong.fetch_add(1, std::memory_order_relaxed);

        auto* derived =
            static_cast< detail::control_block<T, std::default_delete<T>>* >(ctrl_);

        my_shared_ptr<T> sp;        
        sp.ptr_  = derived->ptr;
        sp.ctrl_ = ctrl_;
        return sp;
    }
};
#endif