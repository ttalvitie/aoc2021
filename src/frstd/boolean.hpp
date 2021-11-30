#pragma once

namespace frstd {

struct boolean {
    bool raw;

    boolean() : raw(false) {}
    boolean(bool src) : raw(src) {}

    explicit operator bool() const {
        return raw;
    }

    boolean operator==(boolean other) const { return raw == other.raw; }
    boolean operator!=(boolean other) const { return raw != other.raw; }

    boolean operator&&(boolean other) const { return raw && other.raw; }
    boolean operator||(boolean other) const { return raw || other.raw; }

    boolean operator!() const { return !raw; }
};

}
