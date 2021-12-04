#pragma once

namespace frstd {
namespace driver {

using Size = decltype(sizeof(int));

void abortProgram();

void writeStdout(const unsigned char* data, Size size);
void writeStderr(const unsigned char* data, Size size);

Size readStdin(unsigned char* buf, Size size);

// The size argument must be at least 128 kilobytes.
void* allocateMemory(Size size);
void freeMemory(void* ptr);

}
}
