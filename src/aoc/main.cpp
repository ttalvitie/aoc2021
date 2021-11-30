#include <frstd/prelude.hpp>

void run() {
    DynArray<usz> x;
    for(usz i = 0; 1000 > i; i += 10) {
        x.push(i);
    }
    DynArray<usz> y = x;
    DynArray<usz> z = move(y);
    for(usz i = 0; i < z.size(); ++i) {
        frstd::writeStdout(toString(fromString<usz>(toString(z[i]))) + "\n");
    }

    for(i32 i = -128; i < 128; ++i) {
        frstd::writeStdout(toString(fromString<i8>(toString((i8)i))) + "\n");
        frstd::writeStdout(toString(fromString<i8w>(toString((i8w)i))) + "\n");
    }
    for(i32 i = 0; i < 256; ++i) {
        frstd::writeStdout(toString(fromString<u8>(toString((u8)i))) + "\n");
        frstd::writeStdout(toString(fromString<u8w>(toString((u8w)i))) + "\n");
    }

    frstd::writeStdout(toString((i8)-128) + "\n");
}
