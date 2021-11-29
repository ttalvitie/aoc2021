#include <frstd/prelude.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace frstd {
namespace driver {

void abortProgram() {
    abort();
    __builtin_unreachable();
}

void writeStdout(const char* data, const usz size) {
    if(fwrite(data, 1, size.raw, stdout) != (size_t)size.raw) {
        abortProgram();
    }
}
void writeStderr(const char* data, const usz size) {
    if(fwrite(data, 1, size.raw, stderr) != (size_t)size.raw) {
        abortProgram();
    }
}

void* allocateMemory(const usz size) {
    size_t sizeRaw = size.raw;
    if(sizeRaw == 0) {
        sizeRaw = 1;
    }
    void* ret = malloc(sizeRaw);
    if(ret == nullptr) {
        const char* msg = "FATAL ERROR: Memory allocation failed";
        writeStderr(msg, strlen(msg));
        abortProgram();
    }
    return ret;
}

}
}

// Entry point defined by program.
void run();

int main(int argc, char* argv[]) {
    run();
    return 0;
}
