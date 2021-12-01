#pragma once

#include <frstd/driver.hpp>

namespace frstd {
namespace baseutil {

inline void fail(const char* msg) {
    driver::Size size = 0;
    while(msg[size] != '\0') {
        ++size;
    }
    driver::writeStderr((const unsigned char*)msg, size);
    driver::abortProgram();
    __builtin_unreachable();
}

}
}
