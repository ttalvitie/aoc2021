#pragma once

inline void* operator new(decltype(sizeof(int)), void* ptr) {
    return ptr;
}
