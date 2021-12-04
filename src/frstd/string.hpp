#pragma once

#include <frstd/baseutil.hpp>
#include <frstd/driver.hpp>
#include <frstd/array.hpp>
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

inline usz len(const String& str) {
    return len(str.bytes);
}

inline bool operator==(const String& a, const String& b) {
    return a.bytes == b.bytes;
}
inline bool operator!=(const String& a, const String& b) {
    return a.bytes != b.bytes;
}

inline String operator+(const String& a, const String& b) {
    String ret;
    for(usz i = 0; i < len(a); ++i) {
        ret.push(a[i]);
    }
    for(usz i = 0; i < len(b); ++i) {
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

inline void integerParseError() {
    baseutil::fail("FAIL: Given string cannot be parsed as an integer of given type\n");
}

template <typename T>
T uncheckedIntegerFromStringImpl(const String& str, usz start = 0) {
    T ret = 0;
    for(usz i = start; i < len(str); ++i) {
        u8 ch = str[i];
        if(ch < '0' || ch > '9') {
            integerParseError();
        }
        ret *= 10;
        ret += (T)(ch - '0');
    }
    return ret;
}

template <typename T>
T unsignedIntegerFromStringImpl(const String& str) {
    using Wrap = typename T::UnsignedWrapInt;
    Wrap ret = uncheckedIntegerFromStringImpl<Wrap>(str, 0);
    if(toString(ret) != str) {
        integerParseError();
    }
    return (T)ret;
}

template <typename T>
T signedIntegerFromStringImpl(const String& str) {
    if(len(str) == 0) {
        integerParseError();
    }
    using Wrap = typename T::SignedWrapInt;
    Wrap ret;
    if(str[0] == '-') {
        ret = -uncheckedIntegerFromStringImpl<Wrap>(str, 1);
    } else {
        ret = uncheckedIntegerFromStringImpl<Wrap>(str, 0);
    }
    if(toString(ret) != str) {
        integerParseError();
    }
    return (T)ret;
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

template <typename T>
struct FromString {
    // In specializations, implement the following function:
    static T parse(const String&) = delete;
};

template <typename T>
T fromString(const String& str) {
    return FromString<T>::parse(str);
}

template <> struct FromString<i8> { static i8 parse(const String& str) { return string_::signedIntegerFromStringImpl<i8>(str); } };
template <> struct FromString<i8w> { static i8w parse(const String& str) { return string_::signedIntegerFromStringImpl<i8w>(str); } };
template <> struct FromString<u8> { static u8 parse(const String& str) { return string_::unsignedIntegerFromStringImpl<u8>(str); } };
template <> struct FromString<u8w> { static u8w parse(const String& str) { return string_::unsignedIntegerFromStringImpl<u8w>(str); } };
template <> struct FromString<i16> { static i16 parse(const String& str) { return string_::signedIntegerFromStringImpl<i16>(str); } };
template <> struct FromString<i16w> { static i16w parse(const String& str) { return string_::signedIntegerFromStringImpl<i16w>(str); } };
template <> struct FromString<u16> { static u16 parse(const String& str) { return string_::unsignedIntegerFromStringImpl<u16>(str); } };
template <> struct FromString<u16w> { static u16w parse(const String& str) { return string_::unsignedIntegerFromStringImpl<u16w>(str); } };
template <> struct FromString<i32> { static i32 parse(const String& str) { return string_::signedIntegerFromStringImpl<i32>(str); } };
template <> struct FromString<i32w> { static i32w parse(const String& str) { return string_::signedIntegerFromStringImpl<i32w>(str); } };
template <> struct FromString<u32> { static u32 parse(const String& str) { return string_::unsignedIntegerFromStringImpl<u32>(str); } };
template <> struct FromString<u32w> { static u32w parse(const String& str) { return string_::unsignedIntegerFromStringImpl<u32w>(str); } };
template <> struct FromString<i64> { static i64 parse(const String& str) { return string_::signedIntegerFromStringImpl<i64>(str); } };
template <> struct FromString<i64w> { static i64w parse(const String& str) { return string_::signedIntegerFromStringImpl<i64w>(str); } };
template <> struct FromString<u64> { static u64 parse(const String& str) { return string_::unsignedIntegerFromStringImpl<u64>(str); } };
template <> struct FromString<u64w> { static u64w parse(const String& str) { return string_::unsignedIntegerFromStringImpl<u64w>(str); } };
template <> struct FromString<isz> { static isz parse(const String& str) { return string_::signedIntegerFromStringImpl<isz>(str); } };
template <> struct FromString<iszw> { static iszw parse(const String& str) { return string_::signedIntegerFromStringImpl<iszw>(str); } };
template <> struct FromString<usz> { static usz parse(const String& str) { return string_::unsignedIntegerFromStringImpl<usz>(str); } };
template <> struct FromString<uszw> { static uszw parse(const String& str) { return string_::unsignedIntegerFromStringImpl<uszw>(str); } };

inline bool isWhitespace(u8 byte) {
    return (byte >= 9 && byte <= 13) || (byte >= 28 && byte <= 32);
}

inline String strip(String str) {
    usz start = 0;
    usz end = len(str);
    while(start < end && isWhitespace(str[start])) {
        ++start;
    }
    while(start < end && isWhitespace(str[end - 1])) {
        --end;
    }
    String ret;
    for(usz i = start; i < end; ++i) {
        ret.push(str[i]);
    }
    return ret;
}

inline DynArray<String> split(String str, u8 sep) {
    DynArray<String> ret;
    String acc;
    usz pos = 0;
    while(pos < len(str)) {
        u8 byte = str[pos++];
        if(byte == sep) {
            ret.push(move(acc));
            acc = "";
        } else {
            acc.push(byte);
        }
    }
    ret.push(move(acc));
    return ret;
}

// TODO: move
inline void writeStdout(const String& str) {
    driver::writeStdout((const unsigned char*)str.data(), len(str).raw);
}
inline void writeStderr(const String& str) {
    driver::writeStderr((const unsigned char*)str.data(), len(str).raw);
}
inline String readStdin() {
    String ret;
    u8 buf[40];
    while(true) {
        driver::Size count = driver::readStdin((unsigned char*)buf, 40);
        if(!count) {
            break;
        }
        for(driver::Size i = 0; i < count; ++i) {
            ret.push(buf[i]);
        }
    }
    return ret;
}

}
