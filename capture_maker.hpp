#ifndef CAPTURE_MAKER_H
#define CAPTURE_MAKER_H

#include "capture_backend.hpp"
#include "capture_backend_ip.hpp"
#include "capture_backend_udp.hpp"
#include "capture_backend_fs.hpp"

namespace capture {

enum class backend_type {
    udp,
    tcp,
    serial,
    fs,
};

template <typename ...Args>
backend_t *make_backend(backend_type type, Args... args) {
    switch (type) {
        case backend_type::udp:
            return new backend_udp_t(args...);
        case backend_type::fs:
            return new backend_fs_t(args...);
        case backend_type::serial:
        case backend_type::tcp:
        default:
            return nullptr;
    }
}

}

#endif
