#pragma once

#include <frstd/baseutil.hpp>
#include <frstd/integer.hpp>
#include <frstd/memory.hpp>
#include <frstd/meta.hpp>
#include <frstd/option.hpp>

namespace frstd {

namespace array_ {

inline void indexOutOfBounds() {
#ifdef FRSTD_DEBUG
    baseutil::fail("FAIL: Array index out of bounds\n");
#endif
    __builtin_unreachable();
}

inline void invalidSlice() {
    baseutil::fail("FAIL: Use of invalidated array slice detected\n");
    __builtin_unreachable();
}

}

template <typename T>
class DynArray;

template <typename T>
class ArraySliceImpl;

template <typename T>
using ArraySlice = ArraySliceImpl<const T>;

template <typename T>
using MutArraySlice = ArraySliceImpl<T>;

template <typename T>
class ArraySliceImpl {
public:
    ArraySliceImpl()
        : data_(nullptr)
        , len_(0)
#ifdef FRSTD_DEBUG
        , dataRevisionIdx_(0)
        , revisionIdx_(0)
#endif
    {}

    T& operator[](usz i) const {
        if(i >= len_) {
            array_::indexOutOfBounds();
        }
#ifdef FRSTD_DEBUG
        if(*dataRevisionIdx_ != revisionIdx_) {
            array_::invalidSlice();
        }
#endif
        return data_[i.raw];
    }

private:
    T* data_;
    usz len_;

#ifdef FRSTD_DEBUG
    Rc<usz> dataRevisionIdx_;
    usz revisionIdx_;

    ArraySliceImpl(T* data, usz len, Rc<usz> dataRevisionIdx)
        : data_(data)
        , len_(len)
        , dataRevisionIdx_(dataRevisionIdx)
        , revisionIdx_(*dataRevisionIdx)
    {}
#else
    ArraySliceImpl(T* data, usz len) : data_(data), len_(len) {}
#endif

    template <typename T2>
    friend usz len(const ArraySliceImpl<T2>& arr);
    template <typename T2>
    friend ArraySlice<T2> slice(const DynArray<T2>& arr, usz start, usz end);
    template <typename T2>
    friend MutArraySlice<T2> mutSlice(DynArray<T2>& arr, usz start, usz end);
};

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
        src.revise_();
        data_ = src.data_;
        len_ = src.len_;
        capacity_ = src.capacity_;
        src.data_ = nullptr;
        src.len_ = 0;
        src.capacity_ = 0;
    }
    DynArray& operator=(const DynArray& src) {
        revise_();
        reset_();
        populateFrom_(src);
        return *this;
    }
    DynArray& operator=(DynArray&& src) {
        revise_();
        src.revise_();
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
        revise_();
        reset_();
    }

    void resize(usz len, const T& value) {
        revise_();
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
            array_::indexOutOfBounds();
        }
        return data_[i.raw];
    }
    const T& operator[](usz i) const {
        if(i >= len_) {
            array_::indexOutOfBounds();
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
        revise_();
        enlarge_(len_ + 1);
        new(&data_[len_.raw]) T(move(elem));
        len_ = len_ + 1;
    }

private:
    void reset_() {
        if(data_ != nullptr) {
            for(usz i = 0; i < len_; ++i) {
                data_[i.raw].~T();
            }
            freeMemory(data_, capacity_ * sizeof(T));
        }

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

        T* newData = (T*)allocateMemory(newCapacity * sizeof(T));
        for(usz i = 0; i < len_; ++i) {
            new(&newData[i.raw]) T(move(data_[i.raw]));
            data_[i.raw].~T();
        }

        if(data_ != nullptr) {
            freeMemory(data_, capacity_ * sizeof(T));
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

        data_ = (T*)allocateMemory(capacity_ * sizeof(T));
        for(usz i = 0; i < len_; ++i) {
            new(&data_[i.raw]) T(src.data_[i.raw]);
        }
    }

    T* data_;
    usz len_;
    usz capacity_;

#ifdef FRSTD_DEBUG
    mutable Option<Rc<usz>> revisionIdx_;

    void revise_() {
        if(!isEmpty(revisionIdx_)) {
            ++*getValue(revisionIdx_);
        }
    }
#else
    void revise_() {}
#endif

    template <typename T2>
    friend usz len(const DynArray<T2>& arr);

    template <typename T2>
    friend ArraySlice<T2> slice(const DynArray<T2>& arr, usz start, usz end);
    template <typename T2>
    friend MutArraySlice<T2> mutSlice(DynArray<T2>& arr, usz start, usz end);
};

template <typename T>
usz len(const ArraySliceImpl<T>& arr) {
    return arr.len_;
}

template <typename T>
usz len(const DynArray<T>& arr) {
    return arr.len_;
}

template <typename T>
bool isEmpty(const ArraySliceImpl<T>& arr) {
    return len(arr) == 0;
}

template <typename T>
bool isEmpty(const DynArray<T>& arr) {
    return len(arr) == 0;
}

template <typename T>
ArraySlice<T> slice(const DynArray<T>& arr) {
    return slice(arr, 0);
}
template <typename T>
ArraySlice<T> slice(const DynArray<T>& arr, usz start) {
    return slice(arr, start, len(arr));
}
template <typename T>
ArraySlice<T> slice(const DynArray<T>& arr, usz start, usz end) {
    if(start > end || end > len(arr)) {
        array_::indexOutOfBounds();
    }
#ifdef FRSTD_DEBUG
    if(isEmpty(arr.revisionIdx_)) {
        arr.revisionIdx_ = Rc<usz>(0);
    }
    return ArraySlice<T>(arr.data() + start.raw, end - start, getValue(arr.revisionIdx_));
#else
    return ArraySlice<T>(arr.data() + start.raw, end - start);
#endif
}

template <typename T>
MutArraySlice<T> mutSlice(DynArray<T>& arr) {
    return mutSlice(arr, 0);
}
template <typename T>
MutArraySlice<T> mutSlice(DynArray<T>& arr, usz start) {
    return mutSlice(arr, start, len(arr));
}
template <typename T>
MutArraySlice<T> mutSlice(DynArray<T>& arr, usz start, usz end) {
    if(start > end || end > len(arr)) {
        array_::indexOutOfBounds();
    }
#ifdef FRSTD_DEBUG
    if(isEmpty(arr.revisionIdx_)) {
        arr.revisionIdx_ = Rc<usz>(0);
    }
    return MutArraySlice<T>(arr.data() + start.raw, end - start, getValue(arr.revisionIdx_));
#else
    return MutArraySlice<T>(arr.data() + start.raw, end - start);
#endif
}

template <typename T>
bool operator==(const DynArray<T>& a, const DynArray<T>& b) {
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
bool operator!=(const DynArray<T>& a, const DynArray<T>& b) {
    return !(a == b);
}

}
