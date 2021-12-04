#include <frstd/memory.hpp>

namespace frstd {

namespace {

using driver::Size;

const int MaxPoolAllocExp = 17;
const Size MaxPoolAllocSize = (Size)1 << MaxPoolAllocExp;

struct MemoryPool {
    Size chunkSize;
    Size count;
    void* next;
    void* freeList;
};
MemoryPool memoryPool[MaxPoolAllocExp + 1];

int findAllocExp(Size size) {
    if(size < sizeof(void*)) {
        size = sizeof(void*);
    }
    static_assert(sizeof(unsigned long long) >= sizeof(Size));
    return (int)(8 * sizeof(unsigned long long)) - __builtin_clzll((unsigned long long)(size - 1));
}

}

void* allocateMemory(usz size) {
    if(size.raw > MaxPoolAllocSize) {
        return driver::allocateMemory(size.raw);
    }

    int allocExp = findAllocExp(size.raw);
    Size allocSize = (Size)1 << allocExp;
    MemoryPool& pool = memoryPool[allocExp];

    if(pool.freeList != nullptr) {
        void* ret = pool.freeList;
        pool.freeList = *(void**)pool.freeList;
        return ret;
    }

    if(pool.count == 0) {
        if(pool.chunkSize < MaxPoolAllocSize) {
            pool.chunkSize = MaxPoolAllocSize;
        } else {
            pool.chunkSize *= 2;
        }
        pool.next = driver::allocateMemory(pool.chunkSize);
        pool.count = pool.chunkSize / allocSize;
    }

    void* ret = pool.next;
    pool.next += allocSize;
    --pool.count;
    return ret;
}

void freeMemory(void* ptr, usz size) {
    if(size.raw > MaxPoolAllocSize) {
        driver::freeMemory(ptr);
        return;
    }

    int allocExp = findAllocExp(size.raw);
    MemoryPool& pool = memoryPool[allocExp];

    *(void**)ptr = pool.freeList;
    pool.freeList = ptr;
}

}
