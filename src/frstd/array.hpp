#pragma once

#include <frstd/baseutil.hpp>
#include <frstd/driver.hpp>
#include <frstd/integer.hpp>
#include <frstd/memory.hpp>
#include <frstd/meta.hpp>

namespace frstd {

namespace dynarray_ {

inline void indexOutOfBounds() {
#ifdef FRSTD_DEBUG
    baseutil::fail("FAIL: Array index out of bounds\n");
#endif
    __builtin_unreachable();
}

}

template <typename T>
class DynArray {
public:
    DynArray() : data_(nullptr), len_(0), capacity_(0) {}
    DynArray(usz len, const T& value) : DynArray() {
        resize(len, value);
    }

    DynArray(const DynArray& src) : DynArray() {
        populateFrom_(src);
    }
    DynArray(DynArray&& src) {
        data_ = src.data_;
        len_ = src.len_;
        capacity_ = src.capacity_;
        src.data_ = nullptr;
        src.len_ = 0;
        src.capacity_ = 0;
    }
    DynArray& operator=(const DynArray& src) {
        reset_();
        populateFrom_(src);
        return *this;
    }
    DynArray& operator=(DynArray&& src) {
        reset_();
        data_ = src.data_;
        len_ = src.len_;
        capacity_ = src.capacity_;
        src.data_ = nullptr;
        src.len_ = 0;
        src.capacity_ = 0;
        return *this;
    }

    ~DynArray() {
        reset_();
    }

    void resize(usz len, const T& value) {
        enlarge_(len);

        if(len < len_) {
            for(usz i = len; i < len_; ++i) {
                data_[i.raw].~T();
            }
        } else {
            for(usz i = len_; i < len; ++i) {
                new(&data_[i.raw]) T(value);
            }
        }
        len_ = len;
    }

    T& operator[](usz i) {
        if(i >= len_) {
            dynarray_::indexOutOfBounds();
        }
        return data_[i.raw];
    }
    const T& operator[](usz i) const {
        if(i >= len_) {
            dynarray_::indexOutOfBounds();
        }
        return data_[i.raw];
    }

    T* data() {
        return data_;
    }
    const T* data() const {
        return data_;
    }

    void push(T elem) {
        enlarge_(len_ + 1);
        new(&data_[len_.raw]) T(move(elem));
        len_ = len_ + 1;
    }

private:
    void reset_() {
        // TODO: free
        data_ = nullptr;
        len_ = 0;
        capacity_ = 0;
    }

    void enlarge_(usz minCapacity) {
        if(capacity_ >= minCapacity) {
            return;
        }

        usz newCapacity = capacity_ + capacity_ / 2 + 1;
        if(newCapacity < minCapacity) {
            newCapacity = minCapacity;
        }

        T* newData = (T*)driver::allocateMemory((newCapacity * sizeof(T)).raw);
        for(usz i = 0; i < len_; ++i) {
            new(&newData[i.raw]) T(move(data_[i.raw]));
            data_[i.raw].~T();
        }

        data_ = newData;
        capacity_ = newCapacity;
    }

    void populateFrom_(const DynArray& src) {
        if(src.len_ == 0) {
            return;
        }

        len_ = src.len_;
        capacity_ = src.len_;

        data_ = (T*)driver::allocateMemory((len_ * sizeof(T)).raw);
        for(usz i = 0; i < len_; ++i) {
            new(&data_[i.raw]) T(src.data_[i.raw]);
        }
    }

    T* data_;
    usz len_;
    usz capacity_;

    template <typename T2>
    friend usz len(const DynArray<T2>& arr);
};

template <typename T>
usz len(const DynArray<T>& arr) {
    return arr.len_;
}

template <typename T>
boolean operator==(const DynArray<T>& a, const DynArray<T>& b) {
    if(len(a) != len(b)) {
        return false;
    }
    for(usz i = 0; i < len(a); ++i) {
        if(a[i] != b[i]) {
            return false;
        }
    }
    return true;
}
template <typename T>
boolean operator!=(const DynArray<T>& a, const DynArray<T>& b) {
    return !(a == b);
}

}
