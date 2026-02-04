#ifndef UI_UTILITY_H
#define UI_UTILITY_H

#include <format>
#include <string>
#include "capture_backend.hpp"

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

std::string print_packet_sock(capture::packet_t& pkt) {
    std::string res;
    auto *udp_hdr = reinterpret_cast<
        capture::udp_header_t*
    >(pkt.get_data().data());
    auto *ip_hdr = &udp_hdr->ip;

    res += std::format("{}\t{}\t", 
            print_socket(ip_hdr->ip.saddr, udp_hdr->udp.source),
            print_socket(ip_hdr->ip.daddr, udp_hdr->udp.dest));
    return res;
}

std::string print_packet_message(capture::packet_t& pkt) {
    std::string res;

    auto msg = static_cast<mavlink_message_t>(pkt);
    res += std::format("{}\t{}\t{}\t{}\t",
            msg.sysid, msg.compid, msg.seq,
            static_cast<int>(msg.msgid));

    res += std::format("{}\t", pkt.get_name());
    return res;
}

std::string print_packet_fields(capture::packet_t pkt) {
    std::string res;
    auto fields = pkt.get_fields();
    for (auto &[name, value] : fields) {
        auto visitor = [](const auto& v){
            return std::format("{}", v);
        };

        res += std::format("{}={},", name, 
                std::visit(visitor, value));
    }
    return res;
}

}

#endif
