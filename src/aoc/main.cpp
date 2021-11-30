#include <frstd/prelude.hpp>

#include <cstdio>

void run() {
    DynArray<usz> x;
    for(usz i = 0; i < 1000; i += 10) {
        x.push(i);
    }
    DynArray<usz> y = x;
    DynArray<usz> z = move(y);
    for(usz i = 0; i < z.size(); ++i) {
        frstd::writeStdout(toString(z[i]) + "\n");
    }

    frstd::writeStdout(toString((i8)-128) + "\n");
}
