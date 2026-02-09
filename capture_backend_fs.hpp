#ifndef CAPTURE_BACKEND_FS_H
#define CAPTURE_BACKEND_FS_H

#include <fstream>
#include "capture_backend.hpp"

namespace capture {

class backend_fs_t : public backend_t {
public:
    backend_fs_t(const char *file_name) {
        stream.open(file_name, 
                std::ios::in | std::ios::out | std::ios::binary);
        if (!stream.is_open()) {
            ok = false;
        }
        ok = true;
     }
    ~backend_fs_t() {
        stream.close();
    }

    bool listen() override {
        char byte = 0x00;
        packet_t packet(sizeof(udp_header_t));
        while (stream >> byte) {
            packet.push(byte);

            if (packet.get_out() !=
                    packet_t::outcome::incomplete) {
                break;
            }
        }

        if (packet.get_out() == packet_t::outcome::ok) {
            packets.push_back(packet);
            return true;
        }
        return false;
    } 

    void save(backend_t *other) {
        for (auto& pkt : backend_t::get_packets(other)) {
            pkt.save(stream); 
        } 
    }

    bool should_exit() override {
        return stream.eof();    
    }
private:
    std::fstream stream;
};

}

#endif
