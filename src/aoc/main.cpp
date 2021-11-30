#include <frstd/prelude.hpp>

#include <cstdio>

void run() {
    DynArray<usz> x;
    for(usz i = 0; i < 1000; i = i + 10) {
        x.push(i);
    }
    DynArray<usz> y = x;
    DynArray<usz> z = move(y);
    for(usz i = 0; i < z.size(); i = i + 1) {
        printf("%lld\n", (long long int)z[i].raw);
    }
}
