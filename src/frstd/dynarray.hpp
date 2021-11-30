#pragma once

#include <frstd/driver.hpp>
#include <frstd/integer.hpp>
#include <frstd/meta.hpp>

namespace frstd {

namespace dynarray_ {

inline void indexOutOfBounds() {
#ifdef FRSTD_DEBUG
    const char msg[] = "FAIL: Array index out of bounds\n";
    frstd::driver::writeStderr(msg, sizeof(msg) - 1);
    frstd::driver::abortProgram();
#endif
    __builtin_unreachable();
}

}

template <typename T>
class DynArray {
public:
    DynArray() : data_(nullptr), size_(0), capacity_(0) {}
    DynArray(usz size, const T& value) : DynArray() {
        resize(size, value);
    }

    DynArray(const DynArray& src) : DynArray() {
        populateFrom_(src);
    }
    DynArray(DynArray&& src) {
        data_ = src.data_;
        size_ = src.size_;
        capacity_ = src.capacity_;
        src.data_ = nullptr;
        src.size_ = 0;
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
        size_ = src.size_;
        capacity_ = src.capacity_;
        src.data_ = nullptr;
        src.size_ = 0;
        src.capacity_ = 0;
    }

    ~DynArray() {
        reset_();
    }

    void resize(usz size, const T& value) {
        enlarge_(size);

        if(size < size_) {
            for(usz i = size; i < size_; ++i) {
                data_[i.raw].~T();
            }
        } else {
            for(usz i = size_; i < size; ++i) {
                new(&data_[i.raw]) T(value);
            }
        }
        size_ = size;
    }

    usz size() const {
        return size_;
    }

    T& operator[](usz i) {
        if(i >= size_) {
            dynarray_::indexOutOfBounds();
        }
        return data_[i.raw];
    }
    const T& operator[](usz i) const {
        if(i >= size_) {
            dynarray_::indexOutOfBounds();
        }
        return data_[i.raw];
    }

    void push(T elem) {
        enlarge_(size_ + 1);
        new(&data_[size_.raw]) T(move(elem));
        size_ = size_ + 1;
    }

private:
    void reset_() {
        // TODO: free
        data_ = nullptr;
        size_ = 0;
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

        T* newData = (T*)driver::allocateMemory(newCapacity * sizeof(T));
        for(usz i = 0; i < size_; ++i) {
            new(&newData[i.raw]) T(move(data_[i.raw]));
            data_[i.raw].~T();
        }

        data_ = newData;
        capacity_ = newCapacity;
    }

    void populateFrom_(const DynArray& src) {
        if(src.size_ == 0) {
            return;
        }

        size_ = src.size_;
        capacity_ = src.size_;

        data_ = (T*)driver::allocateMemory(size_ * sizeof(T));
        for(usz i = 0; i < size_; ++i) {
            new(&data_[i.raw]) T(src.data_[i.raw]);
        }
    }

    T* data_;
    usz size_;
    usz capacity_;
};

}
