#pragma once

namespace frstd {

namespace meta_ {

template <typename T>
struct RemoveReferenceImpl {
    using Type = T;
};

template <typename T>
struct RemoveReferenceImpl<T&> {
    using Type = T;
};

template <typename T>
struct RemoveReferenceImpl<T&&> {
    using Type = T;
};

}

template <typename T>
using RemoveReference = typename meta_::RemoveReferenceImpl<T>::Type;

template <typename T>
RemoveReference<T>&& move(T&& obj) {
    return (RemoveReference<T>&&)obj;
}

template <typename T>
T&& forward(RemoveReference<T>& obj) {
    return (T&&)obj;
}
template <typename T>
T&& forward(RemoveReference<T>&& obj) {
    return (T&&)obj;
}

}
