#pragma once

#include <frstd/array.hpp>
#include <frstd/baseutil.hpp>
#include <frstd/driver.hpp>
#include <frstd/iterable.hpp>
#include <frstd/meta.hpp>

namespace frstd {

struct ConstStringSlice : public Iterable {
    ArraySlice<const u8> bytes;

    const u8& operator[](usz i) {
        return bytes[i];
    }
};
struct StringSlice : public Iterable {
    ArraySlice<u8> bytes;

    operator ConstStringSlice() {
        return {{}, bytes};
    }

    u8& operator[](usz i) {
        return bytes[i];
    }
};

struct String : public Iterable {
    DynArray<u8> bytes;

    String() {}
    String(const char* src) {
        while(*src != '\0') {
            push(*src++);
        }
    }

    explicit String(DynArray<u8> src) : bytes(move(src)) {}

    template <typename T, typename = VoidT<typename IsIterable<T>::Yes>>
    explicit String(const T& src) {
        for(u8 c : src) {
            push(c);
        }
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

inline usz len(const String& s) {
    return len(s.bytes);
}
inline usz len(ConstStringSlice s) {
    return len(s.bytes);
}
inline usz len(StringSlice s) {
    return len(s.bytes);
}

inline bool isEmpty(const String& str) {
    return isEmpty(str.bytes);
}

inline bool operator==(const String& a, const String& b) {
    return a.bytes == b.bytes;
}
inline bool operator!=(const String& a, const String& b) {
    return a.bytes != b.bytes;
}

inline ConstStringSlice slice(const String& s, usz start, usz end) {
    return {{}, slice(s.bytes, start, end)};
}
inline ConstStringSlice slice(const String& s, usz start) {
    return slice(s, start, len(s));
}
inline ConstStringSlice slice(const String& s) {
    return slice(s, 0);
}

inline StringSlice slice(String& s, usz start, usz end) {
    return {{}, slice(s.bytes, start, end)};
}
inline StringSlice slice(String& s, usz start) {
    return slice(s, start, len(s));
}
inline StringSlice slice(String& s) {
    return slice(s, 0);
}

inline ConstStringSlice slice(ConstStringSlice s, usz start, usz end) {
    return {{}, slice(s.bytes, start, end)};
}
inline ConstStringSlice slice(ConstStringSlice s, usz start) {
    return slice(s, start, len(s));
}
inline ConstStringSlice slice(ConstStringSlice s) {
    return slice(s, 0);
}

inline StringSlice slice(StringSlice s, usz start, usz end) {
    return {{}, slice(s.bytes, start, end)};
}
inline StringSlice slice(StringSlice s, usz start) {
    return slice(s, start, len(s));
}
inline StringSlice slice(StringSlice s) {
    return slice(s, 0);
}

inline auto createIterator(ConstStringSlice s) {
    return createIterator(s.bytes);
}
inline auto createIterator(StringSlice s) {
    return createIterator(s.bytes);
}
inline auto createIterator(String& a) {
    return createIterator(a.bytes);
}
inline auto createIterator(const String& a) {
    return createIterator(a.bytes);
}
void createIterator(String&& a) = delete;

inline String operator+(const String& a, const String& b) {
    String ret;
    for(u8 c : a) {
        ret.push(c);
    }
    for(u8 c : b) {
        ret.push(c);
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
        return "-" + unsignedIntegerToStringImpl(-(SameSizeUnsignedWrapInt<T>)val);
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
    using Wrap = SameSizeUnsignedWrapInt<T>;
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
    using Wrap = SameSizeSignedWrapInt<T>;
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

namespace string_ {

template <typename T>
auto stripImpl(T s) {
    usz start = 0;
    usz end = len(s);
    while(start < end && isWhitespace(s[start])) {
        ++start;
    }
    while(start < end && isWhitespace(s[end - 1])) {
        --end;
    }
    return slice(s, start, end);
}

}

inline ConstStringSlice strip(ConstStringSlice s) { return string_::stripImpl(s); }
inline StringSlice strip(StringSlice s) { return string_::stripImpl(s); }
inline ConstStringSlice strip(const String& s) { return string_::stripImpl(slice(s)); }
inline StringSlice strip(String& s) { return string_::stripImpl(slice(s)); }
void strip(String&& s) = delete;

template <typename T>
class SplitIterator {
public:
    SplitIterator(T s, u8 sep) : slice_(s), sep_(sep), pos_(0) {}

    bool hasNext() const {
        return pos_ <= len(slice_);
    }

    auto next() {
        usz start = pos_;
        while(pos_ < len(slice_) && slice_[pos_] != sep_) {
            ++pos_;
        }
        usz end = pos_;
        ++pos_;
        return slice(slice_, start, end);
    }

private:
    T slice_;
    u8 sep_;
    usz pos_;
};

template <typename T>
class Split : public Iterable {
public:
    Split(T s, u8 sep) : slice_(s), sep_(sep) {}

private:
    T slice_;
    u8 sep_;

    template <typename T2>
    friend SplitIterator<T2> createIterator(Split<T2> s);
};

template <typename T>
SplitIterator<T> createIterator(Split<T> s) {
    return SplitIterator(s.slice_, s.sep_);
}

inline auto split(ConstStringSlice s, u8 sep) {
    return Split<ConstStringSlice>(s, sep);
}
inline auto split(StringSlice s, u8 sep) {
    return Split<StringSlice>(s, sep);
}
inline auto split(const String& s, u8 sep) {
    return split(slice(s), sep);
}
inline auto split(String& s, u8 sep) {
    return split(slice(s), sep);
}
void split(String&& s, u8 sep) = delete;

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
