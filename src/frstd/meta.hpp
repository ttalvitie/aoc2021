#pragma once

namespace frstd {

template <typename T>
struct RemoveReference {
    using Type = T;
};

template <typename T>
struct RemoveReference<T&> {
    using Type = T;
};

template <typename T>
struct RemoveReference<T&&> {
    using Type = T;
};

template <typename T>
typename RemoveReference<T>::Type&& move(T&& obj) {
    return (typename RemoveReference<T>::Type&&)obj;
}

}
