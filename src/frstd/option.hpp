#pragma once

#include <frstd/baseutil.hpp>
#include <frstd/memory.hpp>

namespace frstd {

template <typename T>
class Option {
public:
    template <typename... Args>
    static Option create(Args&&... args) {
        Option opt;
        opt.hasValue_ = true;
        new(opt.data_) T(forward<Args>(args)...);
        return opt;
    }

    Option() {
        hasValue_ = false;
    }
    Option(T obj) {
        hasValue_ = true;
        new(data_) T(move(obj));
    }
    Option(const Option& src) {
        hasValue_ = src.hasValue_;
        if(hasValue_) {
            new(data_) T(*(const T*)src.data_);
        }
    }
    Option(const T& src) {
        hasValue_ = true;
        new(data_) T(src);
    }
    Option(Option&& src) {
        hasValue_ = src.hasValue_;
        if(hasValue_) {
            new(data_) T(move(*(T*)src.data_));
            ((T*)src.data_)->~T();
        }
        src.hasValue_ = false;
    }
    Option(T&& src) {
        hasValue_ = true;
        new(data_) T(move(src));
    }
    Option& operator=(const Option& src) {
        if(hasValue_) {
            ((T*)data_)->~T();
        }
        hasValue_ = src.hasValue_;
        if(hasValue_) {
            new(data_) T(*(const T*)src.data_);
        }
        return *this;
    }
    Option& operator=(const T& src) {
        if(hasValue_) {
            ((T*)data_)->~T();
        }
        hasValue_ = true;
        new(data_) T(src);
        return *this;
    }
    Option& operator=(Option&& src) {
        if(hasValue_) {
            ((T*)data_)->~T();
        }
        hasValue_ = src.hasValue_;
        if(hasValue_) {
            new(data_) T(move(*(T*)src.data_));
            ((T*)src.data_)->~T();
        }
        src.hasValue_ = false;
        return *this;
    }
    Option& operator=(T&& src) {
        if(hasValue_) {
            ((T*)data_)->~T();
        }
        hasValue_ = true;
        new(data_) T(move(src));
        return *this;
    }
    ~Option() {
        if(hasValue_) {
            ((T*)data_)->~T();
        }
    }

private:
    bool hasValue_;
    alignas(T) char data_[sizeof(T)];

    template <typename T2>
    friend bool isEmpty(const Option<T2>& opt);

    template <typename T2>
    friend T2& getValue(Option<T2>& opt);

    template <typename T2>
    friend const T2& getValue(const Option<T2>& opt);
};

template <typename T>
bool isEmpty(const Option<T>& opt) {
    return !opt.hasValue_;
}

template <typename T>
T& getValue(Option<T>& opt) {
    if(!opt.hasValue_) {
        baseutil::fail("FAIL: getValue called for empty Option\n");
    }
    return *(T*)opt.data_;
}

template <typename T>
const T& getValue(const Option<T>& opt) {
    if(!opt.hasValue_) {
        baseutil::fail("FAIL: getValue called for empty Option\n");
    }
    return *(const T*)opt.data_;
}

}
