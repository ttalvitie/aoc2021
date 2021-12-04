#pragma once

#include <frstd/driver.hpp>
#include <frstd/integer.hpp>

inline void* operator new(decltype(sizeof(int)), void* ptr) {
    return ptr;
}

namespace frstd {

void* allocateMemory(usz size);
void freeMemory(void* ptr, usz size);

// Checks that all set of allocated memory blocks at construction and destruction match exactly.
// Relies on heuristics; may not always catch leaks. Note that global/static initialization may
// lead to allocations that do not follow scpoe structure.
class LeakCheck {
public:
    LeakCheck();
    ~LeakCheck();

    LeakCheck(const LeakCheck&) = delete;
    LeakCheck(LeakCheck&&) = delete;

    LeakCheck& operator=(const LeakCheck&) = delete;
    LeakCheck& operator=(LeakCheck&&) = delete;

private:
    driver::Size allocCount_;
    driver::Size allocSizeSum_;
    driver::Size allocPtrSum_;
};

}
