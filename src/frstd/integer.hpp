#pragma once

#include <frstd/baseutil.hpp>
#include <frstd/driver.hpp>
#include <frstd/meta.hpp>

namespace frstd {

namespace integer_ {

// We currently only support GCC because it promises correct wrapping modulo 2^N when converting to
// signed integers (see https://gcc.gnu.org/onlinedocs/gcc/Integers-implementation.html). We need
// this in the implementation of SignedWrap.
#ifndef __GNUG__
#error Only GCC is supported
#endif
#ifdef __clang__
#error Only GCC is supported
#endif

inline void divByZero() {
#ifdef FRSTD_DEBUG
    baseutil::fail("FAIL: Division by zero occurred\n");
#endif
    __builtin_unreachable();
}

inline void overflow() {
#ifdef FRSTD_DEBUG
    baseutil::fail("FAIL: Integer overflow occurred\n");
#endif
    __builtin_unreachable();
}

template <typename T>
struct IsIntWrapper {};

}

template <typename U, typename S, bool IsSize>
struct UnsignedInt;
template <typename U, typename S, bool IsSize>
struct Signed;
template <typename U, typename S, bool IsSize>
struct UnsignedWrap;
template <typename U, typename S, bool IsSize>
struct SignedWrap;

template <typename U, typename S, bool IsSize>
struct UnsignedInt {
    U raw;

    UnsignedInt() : raw(0) {}

    UnsignedInt(signed int src) { if(__builtin_add_overflow(src, 0, &raw)) { integer_::overflow(); } }
    UnsignedInt(unsigned int src) { if(__builtin_add_overflow(src, 0, &raw)) { integer_::overflow(); } }
    UnsignedInt(signed long int src) { if(__builtin_add_overflow(src, 0, &raw)) { integer_::overflow(); } }
    UnsignedInt(unsigned long int src) { if(__builtin_add_overflow(src, 0, &raw)) { integer_::overflow(); } }
    UnsignedInt(signed long long int src) { if(__builtin_add_overflow(src, 0, &raw)) { integer_::overflow(); } }
    UnsignedInt(unsigned long long int src) { if(__builtin_add_overflow(src, 0, &raw)) { integer_::overflow(); } }

    template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>>
    explicit UnsignedInt(T src) { if(__builtin_add_overflow(src.raw, 0, &raw)) { integer_::overflow(); } }

    bool operator==(UnsignedInt other) const { return raw == other.raw; }
    bool operator!=(UnsignedInt other) const { return raw != other.raw; }
    bool operator<(UnsignedInt other) const { return raw < other.raw; }
    bool operator>(UnsignedInt other) const { return raw > other.raw; }
    bool operator<=(UnsignedInt other) const { return raw <= other.raw; }
    bool operator>=(UnsignedInt other) const { return raw >= other.raw; }

    UnsignedInt operator+(UnsignedInt other) const { UnsignedInt ret; if(__builtin_add_overflow(raw, other.raw, &ret.raw)) { integer_::overflow(); } return ret; }
    UnsignedInt operator-(UnsignedInt other) const { UnsignedInt ret; if(__builtin_sub_overflow(raw, other.raw, &ret.raw)) { integer_::overflow(); } return ret; }
    UnsignedInt operator*(UnsignedInt other) const { UnsignedInt ret; if(__builtin_mul_overflow(raw, other.raw, &ret.raw)) { integer_::overflow(); } return ret; }
    UnsignedInt operator/(UnsignedInt other) const { if(other == 0) { integer_::divByZero(); } UnsignedInt ret; ret.raw = raw / other.raw; return ret; }

    UnsignedInt& operator+=(UnsignedInt other) { *this = *this + other; return *this; }
    UnsignedInt& operator-=(UnsignedInt other) { *this = *this - other; return *this; }
    UnsignedInt& operator*=(UnsignedInt other) { *this = *this * other; return *this; }
    UnsignedInt& operator/=(UnsignedInt other) { *this = *this / other; return *this; }

    UnsignedInt& operator++() { return *this += 1; }
    UnsignedInt& operator--() { return *this -= 1; }
    UnsignedInt operator++(int) { UnsignedInt ret = *this; ++*this; return ret; }
    UnsignedInt operator--(int) { UnsignedInt ret = *this; --*this; return ret; }
};

template <typename U, typename S, bool IsSize>
struct SignedInt {
    S raw;

    SignedInt() : raw(0) {}

    SignedInt(signed int src) { if(__builtin_add_overflow(src, 0, &raw)) { integer_::overflow(); } }
    SignedInt(unsigned int src) { if(__builtin_add_overflow(src, 0, &raw)) { integer_::overflow(); } }
    SignedInt(signed long int src) { if(__builtin_add_overflow(src, 0, &raw)) { integer_::overflow(); } }
    SignedInt(unsigned long int src) { if(__builtin_add_overflow(src, 0, &raw)) { integer_::overflow(); } }
    SignedInt(signed long long int src) { if(__builtin_add_overflow(src, 0, &raw)) { integer_::overflow(); } }
    SignedInt(unsigned long long int src) { if(__builtin_add_overflow(src, 0, &raw)) { integer_::overflow(); } }

    template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>>
    explicit SignedInt(T src) { if(__builtin_add_overflow(src.raw, 0, &raw)) { integer_::overflow(); } }

    bool operator==(SignedInt other) const { return raw == other.raw; }
    bool operator!=(SignedInt other) const { return raw != other.raw; }
    bool operator<(SignedInt other) const { return raw < other.raw; }
    bool operator>(SignedInt other) const { return raw > other.raw; }
    bool operator<=(SignedInt other) const { return raw <= other.raw; }
    bool operator>=(SignedInt other) const { return raw >= other.raw; }

    SignedInt operator-() const { SignedInt ret; if(__builtin_sub_overflow((S)0, raw, &ret.raw)) { integer_::overflow(); } return ret; }
    SignedInt operator+(SignedInt other) const { SignedInt ret; if(__builtin_add_overflow(raw, other.raw, &ret.raw)) { integer_::overflow(); } return ret; }
    SignedInt operator-(SignedInt other) const { SignedInt ret; if(__builtin_sub_overflow(raw, other.raw, &ret.raw)) { integer_::overflow(); } return ret; }
    SignedInt operator*(SignedInt other) const { SignedInt ret; if(__builtin_mul_overflow(raw, other.raw, &ret.raw)) { integer_::overflow(); } return ret; }
    SignedInt operator/(SignedInt other) const { if(other == 0) { integer_::divByZero(); } if(other == -1) { return -(*this); } SignedInt ret; ret.raw = raw / other.raw; return ret; }

    SignedInt& operator+=(SignedInt other) { *this = *this + other; return *this; }
    SignedInt& operator-=(SignedInt other) { *this = *this - other; return *this; }
    SignedInt& operator*=(SignedInt other) { *this = *this * other; return *this; }
    SignedInt& operator/=(SignedInt other) { *this = *this / other; return *this; }

    SignedInt& operator++() { return *this += 1; }
    SignedInt& operator--() { return *this -= 1; }
    SignedInt operator++(int) { SignedInt ret = *this; ++*this; return ret; }
    SignedInt operator--(int) { SignedInt ret = *this; --*this; return ret; }
};

template <typename U, typename S, bool IsSize>
struct UnsignedWrapInt {
    U raw;

    UnsignedWrapInt() : raw(0) {}

    UnsignedWrapInt(signed int src) : raw((U)src) { }
    UnsignedWrapInt(unsigned int src) : raw((U)src) { }
    UnsignedWrapInt(signed long int src) : raw((U)src) { }
    UnsignedWrapInt(unsigned long int src) : raw((U)src) { }
    UnsignedWrapInt(signed long long int src) : raw((U)src) { }
    UnsignedWrapInt(unsigned long long int src) : raw((U)src) { }

    template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>>
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
    UnsignedWrapInt operator/(UnsignedWrapInt other) const { if(other == 0) { integer_::divByZero(); } UnsignedWrapInt ret; ret.raw = raw / other.raw; return ret; }

    UnsignedWrapInt& operator+=(UnsignedWrapInt other) { *this = *this + other; return *this; }
    UnsignedWrapInt& operator-=(UnsignedWrapInt other) { *this = *this - other; return *this; }
    UnsignedWrapInt& operator*=(UnsignedWrapInt other) { *this = *this * other; return *this; }
    UnsignedWrapInt& operator/=(UnsignedWrapInt other) { *this = *this / other; return *this; }

    UnsignedWrapInt& operator++() { return *this += 1; }
    UnsignedWrapInt& operator--() { return *this -= 1; }
    UnsignedWrapInt operator++(int) { UnsignedWrapInt ret = *this; ++*this; return ret; }
    UnsignedWrapInt operator--(int) { UnsignedWrapInt ret = *this; --*this; return ret; }
};

template <typename U, typename S, bool IsSize>
struct SignedWrapInt {
    S raw;

    SignedWrapInt() : raw(0) {}

    SignedWrapInt(signed int src) : raw((S)src) { }
    SignedWrapInt(unsigned int src) : raw((S)src) { }
    SignedWrapInt(signed long int src) : raw((S)src) { }
    SignedWrapInt(unsigned long int src) : raw((S)src) { }
    SignedWrapInt(signed long long int src) : raw((S)src) { }
    SignedWrapInt(unsigned long long int src) : raw((S)src) { }

    template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>>
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
    SignedWrapInt operator/(SignedWrapInt other) const { if(other == 0) { integer_::divByZero(); } if(other == -1) { return -(*this); } SignedWrapInt ret; ret.raw = raw / other.raw; return ret; }

    SignedWrapInt& operator+=(SignedWrapInt other) { *this = *this + other; return *this; }
    SignedWrapInt& operator-=(SignedWrapInt other) { *this = *this - other; return *this; }
    SignedWrapInt& operator*=(SignedWrapInt other) { *this = *this * other; return *this; }
    SignedWrapInt& operator/=(SignedWrapInt other) { *this = *this / other; return *this; }

    SignedWrapInt& operator++() { return *this += 1; }
    SignedWrapInt& operator--() { return *this -= 1; }
    SignedWrapInt operator++(int) { SignedWrapInt ret = *this; ++*this; return ret; }
    SignedWrapInt operator--(int) { SignedWrapInt ret = *this; --*this; return ret; }
};

template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator==(signed int a, T b) { return (T)a == b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator==(unsigned int a, T b) { return (T)a == b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator==(signed long int a, T b) { return (T)a == b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator==(unsigned long int a, T b) { return (T)a == b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator==(signed long long int a, T b) { return (T)a == b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator==(unsigned long long int a, T b) { return (T)a == b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator!=(signed int a, T b) { return (T)a != b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator!=(unsigned int a, T b) { return (T)a != b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator!=(signed long int a, T b) { return (T)a != b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator!=(unsigned long int a, T b) { return (T)a != b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator!=(signed long long int a, T b) { return (T)a != b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator!=(unsigned long long int a, T b) { return (T)a != b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator<(signed int a, T b) { return (T)a < b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator<(unsigned int a, T b) { return (T)a < b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator<(signed long int a, T b) { return (T)a < b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator<(unsigned long int a, T b) { return (T)a < b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator<(signed long long int a, T b) { return (T)a < b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator<(unsigned long long int a, T b) { return (T)a < b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator>(signed int a, T b) { return (T)a > b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator>(unsigned int a, T b) { return (T)a > b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator>(signed long int a, T b) { return (T)a > b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator>(unsigned long int a, T b) { return (T)a > b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator>(signed long long int a, T b) { return (T)a > b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator>(unsigned long long int a, T b) { return (T)a > b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator<=(signed int a, T b) { return (T)a <= b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator<=(unsigned int a, T b) { return (T)a <= b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator<=(signed long int a, T b) { return (T)a <= b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator<=(unsigned long int a, T b) { return (T)a <= b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator<=(signed long long int a, T b) { return (T)a <= b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator<=(unsigned long long int a, T b) { return (T)a <= b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator>=(signed int a, T b) { return (T)a >= b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator>=(unsigned int a, T b) { return (T)a >= b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator>=(signed long int a, T b) { return (T)a >= b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator>=(unsigned long int a, T b) { return (T)a >= b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator>=(signed long long int a, T b) { return (T)a >= b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> bool operator>=(unsigned long long int a, T b) { return (T)a >= b; }

template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator+(signed int a, T b) { return (T)a + b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator+(unsigned int a, T b) { return (T)a + b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator+(signed long int a, T b) { return (T)a + b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator+(unsigned long int a, T b) { return (T)a + b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator+(signed long long int a, T b) { return (T)a + b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator+(unsigned long long int a, T b) { return (T)a + b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator-(signed int a, T b) { return (T)a - b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator-(unsigned int a, T b) { return (T)a - b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator-(signed long int a, T b) { return (T)a - b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator-(unsigned long int a, T b) { return (T)a - b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator-(signed long long int a, T b) { return (T)a - b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator-(unsigned long long int a, T b) { return (T)a - b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator*(signed int a, T b) { return (T)a * b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator*(unsigned int a, T b) { return (T)a * b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator*(signed long int a, T b) { return (T)a * b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator*(unsigned long int a, T b) { return (T)a * b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator*(signed long long int a, T b) { return (T)a * b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator*(unsigned long long int a, T b) { return (T)a * b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator/(signed int a, T b) { return (T)a / b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator/(unsigned int a, T b) { return (T)a / b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator/(signed long int a, T b) { return (T)a / b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator/(unsigned long int a, T b) { return (T)a / b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator/(signed long long int a, T b) { return (T)a / b; }
template <typename T, typename = VoidT<typename integer_::IsIntWrapper<T>::Yes>> T operator/(unsigned long long int a, T b) { return (T)a / b; }

namespace integer_ {

template <typename U, typename S, bool IsSize>
struct Wrappers {
    static_assert(sizeof(U) == sizeof(S));

    static constexpr int ByteCount = sizeof(U);
    static constexpr int BitCount = 8 * ByteCount;

    using UnsignedInt = ::frstd::UnsignedInt<U, S, IsSize>;
    using SignedInt = ::frstd::SignedInt<U, S, IsSize>;
    using UnsignedWrapInt = ::frstd::UnsignedWrapInt<U, S, IsSize>;
    using SignedWrapInt = ::frstd::SignedWrapInt<U, S, IsSize>;
};

template <typename U, typename S, bool IsSize>
struct IsIntWrapper<UnsignedInt<U, S, IsSize>> { using Yes = int; };
template <typename U, typename S, bool IsSize>
struct IsIntWrapper<SignedInt<U, S, IsSize>> { using Yes = int; };
template <typename U, typename S, bool IsSize>
struct IsIntWrapper<UnsignedWrapInt<U, S, IsSize>> { using Yes = int; };
template <typename U, typename S, bool IsSize>
struct IsIntWrapper<SignedWrapInt<U, S, IsSize>> { using Yes = int; };

template <bool IsSize>
using CharWrappers = Wrappers<unsigned char, signed char, IsSize>;
template <bool IsSize>
using ShortWrappers = Wrappers<unsigned short, signed short, IsSize>;
template <bool IsSize>
using IntWrappers = Wrappers<unsigned int, signed int, IsSize>;
template <bool IsSize>
using LongWrappers = Wrappers<unsigned long, signed long, IsSize>;
template <bool IsSize>
using LongLongWrappers = Wrappers<unsigned long long, signed long long, IsSize>;

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

template <bool IsSize, int BitCount>
using TypeChooser = TypeChooserImpl<BitCount, CharWrappers<IsSize>, ShortWrappers<IsSize>, IntWrappers<IsSize>, LongWrappers<IsSize>, LongLongWrappers<IsSize>>;

using I8Wrappers = TypeChooser<false, 8>::Type;
using I16Wrappers = TypeChooser<false, 16>::Type;
using I32Wrappers = TypeChooser<false, 32>::Type;
using I64Wrappers = TypeChooser<false, 64>::Type;
using ISizeWrappers = TypeChooser<true, 8 * sizeof(void*)>::Type;

struct Dummy {};
static_assert(sizeof(void*) == sizeof(Dummy*));
static_assert(sizeof(void*) == sizeof(sizeof(int)));

template <typename T>
struct ToWrappers {};

template <typename U, typename S, bool IsSize>
struct ToWrappers<UnsignedInt<U, S, IsSize>> { using Type = Wrappers<U, S, IsSize>; };
template <typename U, typename S, bool IsSize>
struct ToWrappers<SignedInt<U, S, IsSize>> { using Type = Wrappers<U, S, IsSize>; };
template <typename U, typename S, bool IsSize>
struct ToWrappers<UnsignedWrapInt<U, S, IsSize>> { using Type = Wrappers<U, S, IsSize>; };
template <typename U, typename S, bool IsSize>
struct ToWrappers<SignedWrapInt<U, S, IsSize>> { using Type = Wrappers<U, S, IsSize>; };

}

using i8 = integer_::I8Wrappers::SignedInt;
using i8w = integer_::I8Wrappers::SignedWrapInt;
using u8 = integer_::I8Wrappers::UnsignedInt;
using u8w = integer_::I8Wrappers::UnsignedWrapInt;
using i16 = integer_::I16Wrappers::SignedInt;
using i16w = integer_::I16Wrappers::SignedWrapInt;
using u16 = integer_::I16Wrappers::UnsignedInt;
using u16w = integer_::I16Wrappers::UnsignedWrapInt;
using i32 = integer_::I32Wrappers::SignedInt;
using i32w = integer_::I32Wrappers::SignedWrapInt;
using u32 = integer_::I32Wrappers::UnsignedInt;
using u32w = integer_::I32Wrappers::UnsignedWrapInt;
using i64 = integer_::I64Wrappers::SignedInt;
using i64w = integer_::I64Wrappers::SignedWrapInt;
using u64 = integer_::I64Wrappers::UnsignedInt;
using u64w = integer_::I64Wrappers::UnsignedWrapInt;
using isz = integer_::ISizeWrappers::SignedInt;
using iszw = integer_::ISizeWrappers::SignedWrapInt;
using usz = integer_::ISizeWrappers::UnsignedInt;
using uszw = integer_::ISizeWrappers::UnsignedWrapInt;

template <typename T>
using SameSizeUnsignedInt = typename integer_::ToWrappers<T>::Type::UnsignedInt;
template <typename T>
using SameSizeSignedInt = typename integer_::ToWrappers<T>::Type::SignedInt;
template <typename T>
using SameSizeUnsignedWrapInt = typename integer_::ToWrappers<T>::Type::UnsignedWrapInt;
template <typename T>
using SameSizeSignedWrapInt = typename integer_::ToWrappers<T>::Type::SignedWrapInt;

}
