#pragma once

#include <frstd/integer.hpp>

inline void* operator new(decltype(sizeof(int)), void* ptr) {
    return ptr;
}

namespace frstd {

void* allocateMemory(usz size);
void freeMemory(void* ptr, usz size);

}
