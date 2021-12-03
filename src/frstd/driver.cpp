#include <frstd/driver.hpp>

#include <frstd/baseutil.hpp>
#include <frstd/array.hpp>
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

void writeStdout(const unsigned char* data, Size size) {
    if(fwrite(data, 1, size, stdout) != size) {
        abortProgram();
    }
}
void writeStderr(const unsigned char* data, Size size) {
    if(fwrite(data, 1, size, stderr) != size) {
        abortProgram();
    }
}

Size readStdin(unsigned char* buf, Size size) {
    Size ret = 0;
    while(ret < size) {
        int c = getchar();
        if(c == EOF) {
            break;
        }
        buf[ret++] = (unsigned char)c;
    }
    return ret;
}

void* allocateMemory(Size size) {
    if(size == 0) {
        size = 1;
    }
    void* ret = malloc(size);
    if(ret == nullptr) {
        frstd::baseutil::fail("FAIL: Memory allocation failed");
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
