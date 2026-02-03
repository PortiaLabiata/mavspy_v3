#ifndef UI_UTILITY_H
#define UI_UTILITY_H

#include <format>
#include <string>

extern "C" {
#include <arpa/inet.h>
}

namespace ui {

std::string print_socket(uint32_t ip, uint16_t port) {
    std::string res;
    for (int i = 0; i < 4; i++) {
        res += std::format("{}.", 
                static_cast<int>(ip & 0xFF));
        ip >>= 8;
    }

    res.pop_back();
    res += std::format(":{}", htons(port));

    return res;
}

}

#endif
