#pragma once

namespace frstd {

namespace integer_ {

// We currently only support GCC because it promises correct wrapping modulo 2^N when converting to
// signed integers (see https://gcc.gnu.org/onlinedocs/gcc/Integers-implementation.html). We need
// this in the implementation of SignedWrapInt.
#ifndef __GNUG__
#error Only GCC is supported
#endif
#ifdef __clang__
#error Only GCC is supported
#endif

inline void divByZero();
inline void overflow();

template <typename...>
struct Void {};

template <typename T>
struct IsIntWrapper {};

template <typename U, typename S>
struct UnsignedInt {
    U raw;

    UnsignedInt() : raw(0) {}

    UnsignedInt(signed int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }
    UnsignedInt(unsigned int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }
    UnsignedInt(signed long int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }
    UnsignedInt(unsigned long int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }
    UnsignedInt(signed long long int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }
    UnsignedInt(unsigned long long int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }

    template <typename T, typename = Void<typename IsIntWrapper<T>::Yes>>
    explicit UnsignedInt(T src) { if(__builtin_add_overflow(src.raw, 0, &raw)) { overflow(); } }

    bool operator==(UnsignedInt other) const { return raw == other.raw; }
    bool operator!=(UnsignedInt other) const { return raw != other.raw; }
    bool operator<(UnsignedInt other) const { return raw < other.raw; }
    bool operator>(UnsignedInt other) const { return raw > other.raw; }
    bool operator<=(UnsignedInt other) const { return raw <= other.raw; }
    bool operator>=(UnsignedInt other) const { return raw >= other.raw; }

    UnsignedInt operator+(UnsignedInt other) const { UnsignedInt ret; if(__builtin_add_overflow(raw, other.raw, &ret.raw)) { overflow(); } return ret; }
    UnsignedInt operator-(UnsignedInt other) const { UnsignedInt ret; if(__builtin_sub_overflow(raw, other.raw, &ret.raw)) { overflow(); } return ret; }
    UnsignedInt operator*(UnsignedInt other) const { UnsignedInt ret; if(__builtin_mul_overflow(raw, other.raw, &ret.raw)) { overflow(); } return ret; }
    UnsignedInt operator/(UnsignedInt other) const { if(other == 0) { divByZero(); } UnsignedInt ret; ret.raw = raw / other.raw; return ret; }

    UnsignedInt& operator+=(UnsignedInt other) { *this = *this + other; return *this; }
    UnsignedInt& operator-=(UnsignedInt other) { *this = *this - other; return *this; }
    UnsignedInt& operator*=(UnsignedInt other) { *this = *this * other; return *this; }
    UnsignedInt& operator/=(UnsignedInt other) { *this = *this / other; return *this; }

    UnsignedInt& operator++() { return *this += 1; }
    UnsignedInt& operator--() { return *this -= 1; }
    UnsignedInt operator++(int) { UnsignedInt ret = *this; ++*this; return ret; }
    UnsignedInt operator--(int) { UnsignedInt ret = *this; --*this; return ret; }
};

template <typename U, typename S>
struct SignedInt {
    S raw;

    SignedInt() : raw(0) {}

    SignedInt(signed int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }
    SignedInt(unsigned int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }
    SignedInt(signed long int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }
    SignedInt(unsigned long int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }
    SignedInt(signed long long int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }
    SignedInt(unsigned long long int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }

    template <typename T, typename = Void<typename IsIntWrapper<T>::Yes>>
    explicit SignedInt(T src) { if(__builtin_add_overflow(src.raw, 0, &raw)) { overflow(); } }

    bool operator==(SignedInt other) const { return raw == other.raw; }
    bool operator!=(SignedInt other) const { return raw != other.raw; }
    bool operator<(SignedInt other) const { return raw < other.raw; }
    bool operator>(SignedInt other) const { return raw > other.raw; }
    bool operator<=(SignedInt other) const { return raw <= other.raw; }
    bool operator>=(SignedInt other) const { return raw >= other.raw; }

    SignedInt operator-() const { SignedInt ret; if(__builtin_sub_overflow((S)0, raw, &ret.raw)) { overflow(); } return ret; }
    SignedInt operator+(SignedInt other) const { SignedInt ret; if(__builtin_add_overflow(raw, other.raw, &ret.raw)) { overflow(); } return ret; }
    SignedInt operator-(SignedInt other) const { SignedInt ret; if(__builtin_sub_overflow(raw, other.raw, &ret.raw)) { overflow(); } return ret; }
    SignedInt operator*(SignedInt other) const { SignedInt ret; if(__builtin_mul_overflow(raw, other.raw, &ret.raw)) { overflow(); } return ret; }
    SignedInt operator/(SignedInt other) const { if(other == 0) { divByZero(); } if(other == -1) { return -(*this); } SignedInt ret; ret.raw = raw / other.raw; return ret; }

    SignedInt& operator+=(SignedInt other) { *this = *this + other; return *this; }
    SignedInt& operator-=(SignedInt other) { *this = *this - other; return *this; }
    SignedInt& operator*=(SignedInt other) { *this = *this * other; return *this; }
    SignedInt& operator/=(SignedInt other) { *this = *this / other; return *this; }

    SignedInt& operator++() { return *this += 1; }
    SignedInt& operator--() { return *this -= 1; }
    SignedInt operator++(int) { SignedInt ret = *this; ++*this; return ret; }
    SignedInt operator--(int) { SignedInt ret = *this; --*this; return ret; }
};

template <typename U, typename S>
struct UnsignedWrapInt {
    U raw;

    UnsignedWrapInt() : raw(0) {}

    UnsignedWrapInt(signed int src) : raw((U)src) { }
    UnsignedWrapInt(unsigned int src) : raw((U)src) { }
    UnsignedWrapInt(signed long int src) : raw((U)src) { }
    UnsignedWrapInt(unsigned long int src) : raw((U)src) { }
    UnsignedWrapInt(signed long long int src) : raw((U)src) { }
    UnsignedWrapInt(unsigned long long int src) : raw((U)src) { }

    template <typename T, typename = Void<typename IsIntWrapper<T>::Yes>>
    explicit UnsignedWrapInt(T src) : raw((U)src.raw) { }

    bool operator==(UnsignedWrapInt other) const { return raw == other.raw; }
    bool operator!=(UnsignedWrapInt other) const { return raw != other.raw; }
    bool operator<(UnsignedWrapInt other) const { return raw < other.raw; }
    bool operator>(UnsignedWrapInt other) const { return raw > other.raw; }
    bool operator<=(UnsignedWrapInt other) const { return raw <= other.raw; }
    bool operator>=(UnsignedWrapInt other) const { return raw >= other.raw; }

    UnsignedWrapInt operator-() const { UnsignedWrapInt ret; ret.raw = -raw; return ret; }
    UnsignedWrapInt operator+(UnsignedWrapInt other) const { UnsignedWrapInt ret; ret.raw = raw + other.raw; return ret; }
    UnsignedWrapInt operator-(UnsignedWrapInt other) const { UnsignedWrapInt ret; ret.raw = raw - other.raw; return ret; }
    UnsignedWrapInt operator*(UnsignedWrapInt other) const { UnsignedWrapInt ret; ret.raw = raw * other.raw; return ret; }
    UnsignedWrapInt operator/(UnsignedWrapInt other) const { if(other == 0) { divByZero(); } UnsignedWrapInt ret; ret.raw = raw / other.raw; return ret; }

    UnsignedWrapInt& operator+=(UnsignedWrapInt other) { *this = *this + other; return *this; }
    UnsignedWrapInt& operator-=(UnsignedWrapInt other) { *this = *this - other; return *this; }
    UnsignedWrapInt& operator*=(UnsignedWrapInt other) { *this = *this * other; return *this; }
    UnsignedWrapInt& operator/=(UnsignedWrapInt other) { *this = *this / other; return *this; }

    UnsignedWrapInt& operator++() { return *this += 1; }
    UnsignedWrapInt& operator--() { return *this -= 1; }
    UnsignedWrapInt operator++(int) { UnsignedWrapInt ret = *this; ++*this; return ret; }
    UnsignedWrapInt operator--(int) { UnsignedWrapInt ret = *this; --*this; return ret; }
};

template <typename U, typename S>
struct SignedWrapInt {
    S raw;

    SignedWrapInt() : raw(0) {}

    SignedWrapInt(signed int src) : raw((S)src) { }
    SignedWrapInt(unsigned int src) : raw((S)src) { }
    SignedWrapInt(signed long int src) : raw((S)src) { }
    SignedWrapInt(unsigned long int src) : raw((S)src) { }
    SignedWrapInt(signed long long int src) : raw((S)src) { }
    SignedWrapInt(unsigned long long int src) : raw((S)src) { }

    template <typename T, typename = Void<typename IsIntWrapper<T>::Yes>>
    explicit SignedWrapInt(T src) : raw((S)src.raw) { }

    bool operator==(SignedWrapInt other) const { return raw == other.raw; }
    bool operator!=(SignedWrapInt other) const { return raw != other.raw; }
    bool operator<(SignedWrapInt other) const { return raw < other.raw; }
    bool operator>(SignedWrapInt other) const { return raw > other.raw; }
    bool operator<=(SignedWrapInt other) const { return raw <= other.raw; }
    bool operator>=(SignedWrapInt other) const { return raw >= other.raw; }

    SignedWrapInt operator-() const { SignedWrapInt ret; ret.raw = (S)(-(U)raw); return ret; }
    SignedWrapInt operator+(SignedWrapInt other) const { SignedWrapInt ret; ret.raw = (S)((U)raw + (U)other.raw); return ret; }
    SignedWrapInt operator-(SignedWrapInt other) const { SignedWrapInt ret; ret.raw = (S)((U)raw - (U)other.raw); return ret; }
    SignedWrapInt operator*(SignedWrapInt other) const { SignedWrapInt ret; ret.raw = (S)((U)raw * (U)other.raw); return ret; }
    SignedWrapInt operator/(SignedWrapInt other) const { if(other == 0) { divByZero(); } if(other == -1) { return -(*this); } SignedWrapInt ret; ret.raw = raw / other.raw; return ret; }

    SignedWrapInt& operator+=(SignedWrapInt other) { *this = *this + other; return *this; }
    SignedWrapInt& operator-=(SignedWrapInt other) { *this = *this - other; return *this; }
    SignedWrapInt& operator*=(SignedWrapInt other) { *this = *this * other; return *this; }
    SignedWrapInt& operator/=(SignedWrapInt other) { *this = *this / other; return *this; }

    SignedWrapInt& operator++() { return *this += 1; }
    SignedWrapInt& operator--() { return *this -= 1; }
    SignedWrapInt operator++(int) { SignedWrapInt ret = *this; ++*this; return ret; }
    SignedWrapInt operator--(int) { SignedWrapInt ret = *this; --*this; return ret; }
};

template <typename U, typename S>
struct Wrappers {
    static_assert(sizeof(U) == sizeof(S));

    static constexpr int ByteCount = sizeof(U);
    static constexpr int BitCount = 8 * ByteCount;

    using Unsigned = UnsignedInt<U, S>;
    using Signed = SignedInt<U, S>;
    using UnsignedWrap = UnsignedWrapInt<U, S>;
    using SignedWrap = SignedWrapInt<U, S>;
};

template <typename U, typename S>
struct IsIntWrapper<UnsignedInt<U, S>> { using Yes = int; };
template <typename U, typename S>
struct IsIntWrapper<SignedInt<U, S>> { using Yes = int; };
template <typename U, typename S>
struct IsIntWrapper<UnsignedWrapInt<U, S>> { using Yes = int; };
template <typename U, typename S>
struct IsIntWrapper<SignedWrapInt<U, S>> { using Yes = int; };

using CharWrappers = Wrappers<unsigned char, signed char>;
using ShortWrappers = Wrappers<unsigned short, signed short>;
using IntWrappers = Wrappers<unsigned int, signed int>;
using LongWrappers = Wrappers<unsigned long, signed long>;
using LongLongWrappers = Wrappers<unsigned long long, signed long long>;

template <int BitCount, typename... T>
struct TypeChooserImpl;

template <bool Match, int BitCount, typename FirstT, typename... T>
struct TypeChooserHelper;

template <int BitCount, typename FirstT, typename... T>
struct TypeChooserHelper<true, BitCount, FirstT, T...> {
    using Type = FirstT;
};

template <int BitCount, typename FirstT, typename... T>
struct TypeChooserHelper<false, BitCount, FirstT, T...> {
    using Type = typename TypeChooserImpl<BitCount, T...>::Type;
};

template <int BitCount, typename FirstT, typename... T>
struct TypeChooserImpl<BitCount, FirstT, T...> {
    using Type = typename TypeChooserHelper<FirstT::BitCount == BitCount, BitCount, FirstT, T...>::Type;
};

template <int BitCount>
using TypeChooser = TypeChooserImpl<BitCount, CharWrappers, ShortWrappers, IntWrappers, LongWrappers, LongLongWrappers>;

using I8Wrappers = TypeChooser<8>::Type;
using I16Wrappers = TypeChooser<16>::Type;
using I32Wrappers = TypeChooser<32>::Type;
using I64Wrappers = TypeChooser<64>::Type;
using ISizeWrappers = TypeChooser<8 * sizeof(void*)>::Type;

struct Dummy {};
static_assert(sizeof(void*) == sizeof(Dummy*));

}

using i8 = integer_::I8Wrappers::Signed;
using i8w = integer_::I8Wrappers::SignedWrap;
using u8 = integer_::I8Wrappers::Unsigned;
using u8w = integer_::I8Wrappers::UnsignedWrap;
using i16 = integer_::I16Wrappers::Signed;
using i16w = integer_::I16Wrappers::SignedWrap;
using u16 = integer_::I16Wrappers::Unsigned;
using u16w = integer_::I16Wrappers::UnsignedWrap;
using i32 = integer_::I32Wrappers::Signed;
using i32w = integer_::I32Wrappers::SignedWrap;
using u32 = integer_::I32Wrappers::Unsigned;
using u32w = integer_::I32Wrappers::UnsignedWrap;
using i64 = integer_::I64Wrappers::Signed;
using i64w = integer_::I64Wrappers::SignedWrap;
using u64 = integer_::I64Wrappers::Unsigned;
using u64w = integer_::I64Wrappers::UnsignedWrap;
using isz = integer_::ISizeWrappers::Signed;
using iszw = integer_::ISizeWrappers::SignedWrap;
using usz = integer_::ISizeWrappers::Unsigned;
using uszw = integer_::ISizeWrappers::UnsignedWrap;

}

#include "driver.hpp"

namespace frstd {
namespace integer_ {

inline void divByZero() {
#ifdef FRSTD_DEBUG
    const char msg[] = "FAIL: Division by zero occurred\n";
    frstd::driver::writeStderr(msg, sizeof(msg) - 1);
    frstd::driver::abortProgram();
#endif
    __builtin_unreachable();
}

inline void overflow() {
#ifdef FRSTD_DEBUG
    const char msg[] = "FAIL: Integer overflow occurred\n";
    frstd::driver::writeStderr(msg, sizeof(msg) - 1);
    frstd::driver::abortProgram();
#endif
    __builtin_unreachable();
}

}
}
