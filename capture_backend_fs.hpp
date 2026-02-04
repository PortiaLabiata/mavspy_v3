#ifndef CAPTURE_BACKEND_FS_H
#define CAPTURE_BACKEND_FS_H

#include <fstream>
#include "capture_backend.hpp"

namespace capture {

class backend_fs_t : public backend_t {
public:
    backend_fs_t(const char *file_name) :
        stream(file_name) {}
    ~backend_fs_t() = default;

    bool listen() override {
        char byte = 0x00;
        while (!stream.eof()) {
            packet_t packet(0);
            stream.read(&byte, 1);
            packet.push(byte);

            if (packet.get_out() !=
                    packet_t::outcome::incomplete) {
                break;
            }

            if (packet.get_out() == packet_t::outcome::ok) {
                packets.push_back(packet);
                return true;
            }
        }
        return false;
    } 

    void save(backend_t& other) {
        for (auto& pkt : backend_t::get_packets(other)) {
            pkt.save(stream); 
        } 
    }
private:
    std::fstream stream;
};

}

#endif
