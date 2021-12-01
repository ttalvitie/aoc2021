#include <frstd/driver.hpp>
#include <frstd/dynarray.hpp>
#include <frstd/string.hpp>

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

String readStdin() {
    String ret;
    while(true) {
        int c = getchar();
        if(c == EOF) {
            break;
        }
        ret.push((u8)(u8w)c);
    }
    return ret;
}

void* allocateMemory(const usz size) {
    size_t sizeRaw = size.raw;
    if(sizeRaw == 0) {
        sizeRaw = 1;
    }
    void* ret = malloc(sizeRaw);
    if(ret == nullptr) {
        const char* msg = "FAIL: Memory allocation failed";
        writeStderr(msg, strlen(msg));
        abortProgram();
    }
    return ret;
}

}
}

// Entry point defined by program.
void run(frstd::DynArray<frstd::String> args);

int main(int argc, char* argv[]) {
    frstd::DynArray<frstd::String> args;
    for(int i = 1; i < argc; ++i) {
        args.push(argv[i]);
    }
    run(move(args));
    return 0;
}
