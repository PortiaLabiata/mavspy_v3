#ifndef CAPTURE_MAKER_H
#define CAPTURE_MAKER_H

#include "capture_backend.hpp"
#include "capture_backend_ip.hpp"
#include "capture_backend_udp.hpp"
#include "capture_backend_fs.hpp"

namespace capture {

template <typename ...Args>
backend_t *make_backend(backend_type type, Args... args) {
    backend_t *back = nullptr;
    switch (type) {
        case backend_type::udp:
            back = new backend_udp_t(args...);
            break;
        case backend_type::fs:
            back = new backend_fs_t(args...);
            break;
        case backend_type::serial:
        case backend_type::tcp:
        default:
            return nullptr;
    }
    return back;
}

}

#endif
