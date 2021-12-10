#pragma once

namespace frstd {

class Iterable {};

namespace iterator_ {

struct IterableMarker {};
struct NotIterableMarker {};

IterableMarker testFunc(Iterable*);
NotIterableMarker testFunc(void*);

}

template <typename T, typename = decltype(iterator_::testFunc((T*)nullptr))>
struct IsIterable {};
template <typename T>
struct IsIterable<T, iterator_::IterableMarker> {
    using Yes = int;
};

}
