#pragma once

#include <frstd/boolean.hpp>

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

inline void divByZero();
inline void overflow();

template <typename...>
struct Void {};

template <typename T>
struct IsIntWrapper {};

template <typename U, typename S>
struct Unsigned;
template <typename U, typename S>
struct Signed;
template <typename U, typename S>
struct UnsignedWrap;
template <typename U, typename S>
struct SignedWrap;

template <typename U, typename S>
struct Unsigned {
    using UnsignedInt = Unsigned<U, S>;
    using SignedInt = Signed<U, S>;
    using UnsignedWrapInt = UnsignedWrap<U, S>;
    using SignedWrapInt = SignedWrap<U, S>;

    U raw;

    Unsigned() : raw(0) {}

    Unsigned(signed int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }
    Unsigned(unsigned int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }
    Unsigned(signed long int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }
    Unsigned(unsigned long int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }
    Unsigned(signed long long int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }
    Unsigned(unsigned long long int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }

    template <typename T, typename = Void<typename IsIntWrapper<T>::Yes>>
    explicit Unsigned(T src) { if(__builtin_add_overflow(src.raw, 0, &raw)) { overflow(); } }

    boolean operator==(Unsigned other) const { return raw == other.raw; }
    boolean operator!=(Unsigned other) const { return raw != other.raw; }
    boolean operator<(Unsigned other) const { return raw < other.raw; }
    boolean operator>(Unsigned other) const { return raw > other.raw; }
    boolean operator<=(Unsigned other) const { return raw <= other.raw; }
    boolean operator>=(Unsigned other) const { return raw >= other.raw; }

    Unsigned operator+(Unsigned other) const { Unsigned ret; if(__builtin_add_overflow(raw, other.raw, &ret.raw)) { overflow(); } return ret; }
    Unsigned operator-(Unsigned other) const { Unsigned ret; if(__builtin_sub_overflow(raw, other.raw, &ret.raw)) { overflow(); } return ret; }
    Unsigned operator*(Unsigned other) const { Unsigned ret; if(__builtin_mul_overflow(raw, other.raw, &ret.raw)) { overflow(); } return ret; }
    Unsigned operator/(Unsigned other) const { if(other == 0) { divByZero(); } Unsigned ret; ret.raw = raw / other.raw; return ret; }

    Unsigned& operator+=(Unsigned other) { *this = *this + other; return *this; }
    Unsigned& operator-=(Unsigned other) { *this = *this - other; return *this; }
    Unsigned& operator*=(Unsigned other) { *this = *this * other; return *this; }
    Unsigned& operator/=(Unsigned other) { *this = *this / other; return *this; }

    Unsigned& operator++() { return *this += 1; }
    Unsigned& operator--() { return *this -= 1; }
    Unsigned operator++(int) { Unsigned ret = *this; ++*this; return ret; }
    Unsigned operator--(int) { Unsigned ret = *this; --*this; return ret; }
};

template <typename U, typename S>
struct Signed {
    using UnsignedInt = Unsigned<U, S>;
    using SignedInt = Signed<U, S>;
    using UnsignedWrapInt = UnsignedWrap<U, S>;
    using SignedWrapInt = SignedWrap<U, S>;

    S raw;

    Signed() : raw(0) {}

    Signed(signed int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }
    Signed(unsigned int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }
    Signed(signed long int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }
    Signed(unsigned long int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }
    Signed(signed long long int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }
    Signed(unsigned long long int src) { if(__builtin_add_overflow(src, 0, &raw)) { overflow(); } }

    template <typename T, typename = Void<typename IsIntWrapper<T>::Yes>>
    explicit Signed(T src) { if(__builtin_add_overflow(src.raw, 0, &raw)) { overflow(); } }

    boolean operator==(Signed other) const { return raw == other.raw; }
    boolean operator!=(Signed other) const { return raw != other.raw; }
    boolean operator<(Signed other) const { return raw < other.raw; }
    boolean operator>(Signed other) const { return raw > other.raw; }
    boolean operator<=(Signed other) const { return raw <= other.raw; }
    boolean operator>=(Signed other) const { return raw >= other.raw; }

    Signed operator-() const { Signed ret; if(__builtin_sub_overflow((S)0, raw, &ret.raw)) { overflow(); } return ret; }
    Signed operator+(Signed other) const { Signed ret; if(__builtin_add_overflow(raw, other.raw, &ret.raw)) { overflow(); } return ret; }
    Signed operator-(Signed other) const { Signed ret; if(__builtin_sub_overflow(raw, other.raw, &ret.raw)) { overflow(); } return ret; }
    Signed operator*(Signed other) const { Signed ret; if(__builtin_mul_overflow(raw, other.raw, &ret.raw)) { overflow(); } return ret; }
    Signed operator/(Signed other) const { if(other == 0) { divByZero(); } if(other == -1) { return -(*this); } Signed ret; ret.raw = raw / other.raw; return ret; }

    Signed& operator+=(Signed other) { *this = *this + other; return *this; }
    Signed& operator-=(Signed other) { *this = *this - other; return *this; }
    Signed& operator*=(Signed other) { *this = *this * other; return *this; }
    Signed& operator/=(Signed other) { *this = *this / other; return *this; }

    Signed& operator++() { return *this += 1; }
    Signed& operator--() { return *this -= 1; }
    Signed operator++(int) { Signed ret = *this; ++*this; return ret; }
    Signed operator--(int) { Signed ret = *this; --*this; return ret; }
};

template <typename U, typename S>
struct UnsignedWrap {
    using UnsignedInt = Unsigned<U, S>;
    using SignedInt = Signed<U, S>;
    using UnsignedWrapInt = UnsignedWrap<U, S>;
    using SignedWrapInt = SignedWrap<U, S>;

    U raw;

    UnsignedWrap() : raw(0) {}

    UnsignedWrap(signed int src) : raw((U)src) { }
    UnsignedWrap(unsigned int src) : raw((U)src) { }
    UnsignedWrap(signed long int src) : raw((U)src) { }
    UnsignedWrap(unsigned long int src) : raw((U)src) { }
    UnsignedWrap(signed long long int src) : raw((U)src) { }
    UnsignedWrap(unsigned long long int src) : raw((U)src) { }

    template <typename T, typename = Void<typename IsIntWrapper<T>::Yes>>
    explicit UnsignedWrap(T src) : raw((U)src.raw) { }

    boolean operator==(UnsignedWrap other) const { return raw == other.raw; }
    boolean operator!=(UnsignedWrap other) const { return raw != other.raw; }
    boolean operator<(UnsignedWrap other) const { return raw < other.raw; }
    boolean operator>(UnsignedWrap other) const { return raw > other.raw; }
    boolean operator<=(UnsignedWrap other) const { return raw <= other.raw; }
    boolean operator>=(UnsignedWrap other) const { return raw >= other.raw; }

    UnsignedWrap operator-() const { UnsignedWrap ret; ret.raw = -raw; return ret; }
    UnsignedWrap operator+(UnsignedWrap other) const { UnsignedWrap ret; ret.raw = raw + other.raw; return ret; }
    UnsignedWrap operator-(UnsignedWrap other) const { UnsignedWrap ret; ret.raw = raw - other.raw; return ret; }
    UnsignedWrap operator*(UnsignedWrap other) const { UnsignedWrap ret; ret.raw = raw * other.raw; return ret; }
    UnsignedWrap operator/(UnsignedWrap other) const { if(other == 0) { divByZero(); } UnsignedWrap ret; ret.raw = raw / other.raw; return ret; }

    UnsignedWrap& operator+=(UnsignedWrap other) { *this = *this + other; return *this; }
    UnsignedWrap& operator-=(UnsignedWrap other) { *this = *this - other; return *this; }
    UnsignedWrap& operator*=(UnsignedWrap other) { *this = *this * other; return *this; }
    UnsignedWrap& operator/=(UnsignedWrap other) { *this = *this / other; return *this; }

    UnsignedWrap& operator++() { return *this += 1; }
    UnsignedWrap& operator--() { return *this -= 1; }
    UnsignedWrap operator++(int) { UnsignedWrap ret = *this; ++*this; return ret; }
    UnsignedWrap operator--(int) { UnsignedWrap ret = *this; --*this; return ret; }
};

template <typename U, typename S>
struct SignedWrap {
    using UnsignedInt = Unsigned<U, S>;
    using SignedInt = Signed<U, S>;
    using UnsignedWrapInt = UnsignedWrap<U, S>;
    using SignedWrapInt = SignedWrap<U, S>;

    S raw;

    SignedWrap() : raw(0) {}

    SignedWrap(signed int src) : raw((S)src) { }
    SignedWrap(unsigned int src) : raw((S)src) { }
    SignedWrap(signed long int src) : raw((S)src) { }
    SignedWrap(unsigned long int src) : raw((S)src) { }
    SignedWrap(signed long long int src) : raw((S)src) { }
    SignedWrap(unsigned long long int src) : raw((S)src) { }

    template <typename T, typename = Void<typename IsIntWrapper<T>::Yes>>
    explicit SignedWrap(T src) : raw((S)src.raw) { }

    boolean operator==(SignedWrap other) const { return raw == other.raw; }
    boolean operator!=(SignedWrap other) const { return raw != other.raw; }
    boolean operator<(SignedWrap other) const { return raw < other.raw; }
    boolean operator>(SignedWrap other) const { return raw > other.raw; }
    boolean operator<=(SignedWrap other) const { return raw <= other.raw; }
    boolean operator>=(SignedWrap other) const { return raw >= other.raw; }

    SignedWrap operator-() const { SignedWrap ret; ret.raw = (S)(-(U)raw); return ret; }
    SignedWrap operator+(SignedWrap other) const { SignedWrap ret; ret.raw = (S)((U)raw + (U)other.raw); return ret; }
    SignedWrap operator-(SignedWrap other) const { SignedWrap ret; ret.raw = (S)((U)raw - (U)other.raw); return ret; }
    SignedWrap operator*(SignedWrap other) const { SignedWrap ret; ret.raw = (S)((U)raw * (U)other.raw); return ret; }
    SignedWrap operator/(SignedWrap other) const { if(other == 0) { divByZero(); } if(other == -1) { return -(*this); } SignedWrap ret; ret.raw = raw / other.raw; return ret; }

    SignedWrap& operator+=(SignedWrap other) { *this = *this + other; return *this; }
    SignedWrap& operator-=(SignedWrap other) { *this = *this - other; return *this; }
    SignedWrap& operator*=(SignedWrap other) { *this = *this * other; return *this; }
    SignedWrap& operator/=(SignedWrap other) { *this = *this / other; return *this; }

    SignedWrap& operator++() { return *this += 1; }
    SignedWrap& operator--() { return *this -= 1; }
    SignedWrap operator++(int) { SignedWrap ret = *this; ++*this; return ret; }
    SignedWrap operator--(int) { SignedWrap ret = *this; --*this; return ret; }
};

template <typename U, typename S>
struct Wrappers {
    static_assert(sizeof(U) == sizeof(S));

    static constexpr int ByteCount = sizeof(U);
    static constexpr int BitCount = 8 * ByteCount;

    using UnsignedInt = Unsigned<U, S>;
    using SignedInt = Signed<U, S>;
    using UnsignedWrapInt = UnsignedWrap<U, S>;
    using SignedWrapInt = SignedWrap<U, S>;
};

template <typename U, typename S>
struct IsIntWrapper<Unsigned<U, S>> { using Yes = int; };
template <typename U, typename S>
struct IsIntWrapper<Signed<U, S>> { using Yes = int; };
template <typename U, typename S>
struct IsIntWrapper<UnsignedWrap<U, S>> { using Yes = int; };
template <typename U, typename S>
struct IsIntWrapper<SignedWrap<U, S>> { using Yes = int; };

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
