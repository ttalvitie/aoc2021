#pragma once

namespace frstd {

namespace integer_ {

template <typename U, typename S>
struct Wrappers {
    static_assert(sizeof(U) == sizeof(S));

    static constexpr int ByteCount = sizeof(U);
    static constexpr int BitCount = 8 * ByteCount;

    using RawUnsigned = U;
    using RawSigned = S;

    struct UnsignedInt {
        U raw;
    };
    struct SignedInt {
        S raw;
    };
};

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
using ISizeWrappers = TypeChooser<sizeof(void*)>::Type;

struct Dummy {};
static_assert(sizeof(void*) == sizeof(Dummy*));

}

}
