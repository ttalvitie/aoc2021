#include <frstd/memory.hpp>

#include <frstd/baseutil.hpp>

namespace frstd {

namespace {

using driver::Size;

const int MaxPoolAllocExp = 17;
const Size MaxPoolAllocSize = (Size)1 << MaxPoolAllocExp;

static_assert(sizeof(char) == 1);

struct MemoryPool {
    Size chunkSize;
    Size count;
    char* next;
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

Size allocCount;
Size allocSizeSum;
Size allocPtrSum;

}

void* allocateMemory(usz size) {
    void* ret;
    if(size.raw > MaxPoolAllocSize) {
        ret = driver::allocateMemory(size.raw);
    } else {
        int allocExp = findAllocExp(size.raw);
        Size allocSize = (Size)1 << allocExp;
        MemoryPool& pool = memoryPool[allocExp];

        if(pool.freeList != nullptr) {
            ret = pool.freeList;
            pool.freeList = *(void**)pool.freeList;
        } else {
            if(pool.count == 0) {
                if(pool.chunkSize < MaxPoolAllocSize) {
                    pool.chunkSize = MaxPoolAllocSize;
                } else {
                    pool.chunkSize *= 2;
                }
                pool.next = (char*)driver::allocateMemory(pool.chunkSize);
                pool.count = pool.chunkSize / allocSize;
            }

            ret = (void*)pool.next;
            pool.next += allocSize;
            --pool.count;
        }
    }

    ++allocCount;
    allocSizeSum += size.raw;
    allocPtrSum += (Size)ret;

    return ret;
}

void freeMemory(void* ptr, usz size) {
    --allocCount;
    allocSizeSum -= size.raw;
    allocPtrSum -= (Size)ptr;

    if(size.raw > MaxPoolAllocSize) {
        driver::freeMemory(ptr);
        return;
    }

    int allocExp = findAllocExp(size.raw);
    MemoryPool& pool = memoryPool[allocExp];

    *(void**)ptr = pool.freeList;
    pool.freeList = ptr;
}

LeakCheck::LeakCheck()
    : allocCount_(allocCount)
    , allocSizeSum_(allocSizeSum)
    , allocPtrSum_(allocPtrSum)
{}

LeakCheck::~LeakCheck() {
    if(allocCount != allocCount_) {
        baseutil::fail("FAIL: Memory leak check failed (allocation count mismatch)\n");
    }
    if(allocSizeSum != allocSizeSum_) {
        baseutil::fail("FAIL: Memory leak check failed (total allocation size mismatch)\n");
    }
    if(allocPtrSum != allocPtrSum_) {
        baseutil::fail("FAIL: Memory leak check failed (allocation pointer sum mismatch)\n");
    }
}

}
