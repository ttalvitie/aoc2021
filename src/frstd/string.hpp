#pragma once

#include <frstd/driver.hpp>
#include <frstd/dynarray.hpp>
#include <frstd/meta.hpp>

namespace frstd {

struct String {
    DynArray<u8> bytes;

    String() {}
    String(const char* src) {
        while(*src != '\0') {
            push(*src++);
        }
    }

    explicit String(DynArray<u8> src) : bytes(move(src)) {}

    usz size() const {
        return bytes.size();
    }

    u8& operator[](usz i) {
        return bytes[i];
    }
    u8 operator[](usz i) const {
        return bytes[i];
    }

    u8* data() {
        return bytes.data();
    }
    const u8* data() const {
        return bytes.data();
    }

    void push(u8 byte) {
        bytes.push(byte);
    }
};

inline String operator+(const String& a, const String& b) {
    String ret;
    for(usz i = 0; i < a.size(); ++i) {
        ret.push(a[i]);
    }
    for(usz i = 0; i < b.size(); ++i) {
        ret.push(b[i]);
    }
    return ret;
}

namespace string_ {

template <typename T>
void unsignedIntegerToStringImplRecursion(T val, String& ret) {
    if(val == 0) {
        return;
    }

    T rest = val / 10;
    unsignedIntegerToStringImplRecursion(rest, ret);
    u8 last = (u8)(val - 10 * rest);
    ret.push('0' + last);
}

template <typename T>
String unsignedIntegerToStringImpl(T val) {
    if(val == 0) {
        return "0";
    } else {
        String ret;
        unsignedIntegerToStringImplRecursion(val, ret);
        return ret;
    }
}

template <typename T>
String integerToStringImpl(T val) {
    if(val < 0) {
        return "-" + unsignedIntegerToStringImpl(-(typename T::UnsignedWrapInt)val);
    } else {
        return unsignedIntegerToStringImpl(val);
    }
}

}

// To make ADL work, we need to implement the toString functions for integers in the integer_
// subnamespace as the integer classes are defined there
namespace integer_ {

inline String toString(i8 val) { return string_::integerToStringImpl(val); }
inline String toString(i8w val) { return string_::integerToStringImpl(val); }
inline String toString(u8 val) { return string_::unsignedIntegerToStringImpl(val); }
inline String toString(u8w val) { return string_::unsignedIntegerToStringImpl(val); }
inline String toString(i16 val) { return string_::integerToStringImpl(val); }
inline String toString(i16w val) { return string_::integerToStringImpl(val); }
inline String toString(u16 val) { return string_::unsignedIntegerToStringImpl(val); }
inline String toString(u16w val) { return string_::unsignedIntegerToStringImpl(val); }
inline String toString(i32 val) { return string_::integerToStringImpl(val); }
inline String toString(i32w val) { return string_::integerToStringImpl(val); }
inline String toString(u32 val) { return string_::unsignedIntegerToStringImpl(val); }
inline String toString(u32w val) { return string_::unsignedIntegerToStringImpl(val); }
inline String toString(i64 val) { return string_::integerToStringImpl(val); }
inline String toString(i64w val) { return string_::integerToStringImpl(val); }
inline String toString(u64 val) { return string_::unsignedIntegerToStringImpl(val); }
inline String toString(u64w val) { return string_::unsignedIntegerToStringImpl(val); }
inline String toString(isz val) { return string_::integerToStringImpl(val); }
inline String toString(iszw val) { return string_::integerToStringImpl(val); }
inline String toString(usz val) { return string_::unsignedIntegerToStringImpl(val); }
inline String toString(uszw val) { return string_::unsignedIntegerToStringImpl(val); }

}

// TODO: move
inline void writeStdout(const String& str) {
    frstd::driver::writeStdout((const char*)str.data(), str.size());
}
inline void writeStderr(const String& str) {
    frstd::driver::writeStdout((const char*)str.data(), str.size());
}

}
