#pragma once

#include <frstd/baseutil.hpp>
#include <frstd/meta.hpp>

namespace frstd {

class Iterable {};

namespace iterator_ {

struct IterableMarker {};
struct NotIterableMarker {};

IterableMarker testFunc(const Iterable*);
NotIterableMarker testFunc(const void*);

}

template <typename T, typename = decltype(iterator_::testFunc((RemoveReference<T>*)nullptr))>
struct IsIterable {};
template <typename T>
struct IsIterable<T, iterator_::IterableMarker> {
    using Yes = int;
};

class RangeForEndIteratorAdapter {};
template <typename T>
class RangeForIteratorAdapter {
public:
    RangeForIteratorAdapter(T iterator) : iterator_(move(iterator)) {}

    RangeForIteratorAdapter(const RangeForIteratorAdapter&) = delete;
    RangeForIteratorAdapter(RangeForIteratorAdapter&&) = delete;
    RangeForIteratorAdapter& operator=(const RangeForIteratorAdapter&) = delete;
    RangeForIteratorAdapter& operator=(RangeForIteratorAdapter&&) = delete;

    decltype(auto) operator*() {
#ifdef FRSTD_DEBUG
        if(!ready_) {
            baseutil::fail("FAIL: Call to range for iterator adapter operator* not allowed in this state");
        }
        ready_ = false;
#endif
        return iterator_.next();
    }
    RangeForIteratorAdapter<T>& operator++() {
#ifdef FRSTD_DEBUG
        if(ready_) {
            baseutil::fail("FAIL: Call to range for iterator adapter operator++ not allowed in this state");
        }
        ready_ = true;
#endif
        return *this;
    }

private:
    T iterator_;
#ifdef FRSTD_DEBUG
    bool ready_ = true;
#endif

    template <typename T2>
    friend bool operator!=(const RangeForIteratorAdapter<T2>& a, const RangeForEndIteratorAdapter&);
};

template <typename T>
bool operator!=(const RangeForIteratorAdapter<T>& a, const RangeForEndIteratorAdapter&) {
    return a.iterator_.hasNext();
}

template <typename T, typename = VoidT<typename IsIterable<T>::Yes>>
auto begin(T&& obj) {
    return RangeForIteratorAdapter(createIterator(forward<T>(obj)));
}
template <typename T, typename = VoidT<typename IsIterable<T>::Yes>>
RangeForEndIteratorAdapter end(T&&) {
    return RangeForEndIteratorAdapter();
}

}
