#pragma once

#include <frstd/driver.hpp>
#include <frstd/integer.hpp>
#include <frstd/meta.hpp>

inline void* operator new(decltype(sizeof(int)), void* ptr) {
    return ptr;
}

namespace frstd {

void* allocateMemory(usz size);
void freeMemory(void* ptr, usz size);

// Checks that all set of allocated memory blocks at construction and destruction match exactly.
// Relies on heuristics; may not always catch leaks. Note that global/static initialization may
// lead to allocations that do not follow scpoe structure.
class LeakCheck {
public:
    LeakCheck();
    ~LeakCheck();

    LeakCheck(const LeakCheck&) = delete;
    LeakCheck(LeakCheck&&) = delete;

    LeakCheck& operator=(const LeakCheck&) = delete;
    LeakCheck& operator=(LeakCheck&&) = delete;

private:
    driver::Size allocCount_;
    driver::Size allocSizeSum_;
    driver::Size allocPtrSum_;
};

template <typename T>
class Rc {
public:
    template <typename... Args>
    static Rc create(Args&&... args) {
        Rc rc;
        rc.holder_ = (Holder*)allocateMemory(sizeof(Holder));
        rc.holder_->refCount = 1;
        new(&rc.holder_->data) T(forward<Args>(args)...);
        return rc;
    }

    Rc(T obj) {
        holder_ = (Holder*)allocateMemory(sizeof(Holder));
        holder_->refCount = 1;
        new(&holder_->data) T(move(obj));
    }
    Rc(const Rc& src) {
        holder_ = src.holder_;
        ++holder_->refCount;
    }
    Rc& operator=(const Rc& src) {
        --holder_->refCount;
        if(holder_->refCount == 0) {
            ((T*)&holder_->data)->~T();
            freeMemory((void*)holder_, sizeof(Holder));
        }
        holder_ = src.holder_;
        ++holder_->refCount;
        return *this;
    }
    ~Rc() {
        --holder_->refCount;
        if(holder_->refCount == 0) {
            ((T*)&holder_->data)->~T();
            freeMemory((void*)holder_, sizeof(Holder));
        }
    }

    T& operator*() const {
        return *(T*)&holder_->data;
    }
    T* operator->() const {
        return (T*)&holder_->data;
    }

private:
    Rc() {}

    struct Holder {
        alignas(T) char data[sizeof(T)];
        usz refCount;
    };

    Holder* holder_;
};

}
