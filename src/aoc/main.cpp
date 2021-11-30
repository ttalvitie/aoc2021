#include <frstd/prelude.hpp>

void run() {
    DynArray<usz> x;
    for(usz i = 0; 1000 > i; i += 10) {
        x.push(i);
    }
    DynArray<usz> y = x;
    DynArray<usz> z = move(y);
    for(usz i = 0; i < z.size(); ++i) {
        frstd::writeStdout(toString(z[i]) + "\n");
    }

    frstd::writeStdout(toString((i8)-128) + "\n");
}
