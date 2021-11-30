#include <frstd/integer.hpp>

// Cannot use #pragma once guard due to cyclic depedendency with frstd/integer.hpp.
#ifndef FRSTD_DRIVER_HPP
#define FRSTD_DRIVER_HPP

namespace frstd {
namespace driver {

void abortProgram();

void writeStdout(const char* data, const usz size);
void writeStderr(const char* data, const usz size);

void* allocateMemory(const usz size);

}
}

#endif
