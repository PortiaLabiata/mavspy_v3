#ifndef CAPTURE_BACKEND_UDP_H
#define CAPTURE_BACKEND_UDP_H

extern "C" {
#include <netinet/udp.h>
}

#include "capture_backend.hpp"
#include "capture_backend_ip.hpp"

namespace capture {

struct udp_header_t {
    ip_header_t ip;
    udphdr udp;
} __attribute__((packed));

class backend_udp_t : public backend_ip_t {
public:
    backend_udp_t(const char *dev) :
         backend_ip_t(dev) {
        if (!ok) {
            return;
        }

        struct bpf_program bpf;
	    ok = pcap_compile(handle, &bpf, "udp", 0, net) == 0;
        if (!ok) {
            return;
        }

	    ok = pcap_setfilter(handle, &bpf) == 0;
     }
    ~backend_udp_t() = default;

    bool listen() override {
        struct pcap_pkthdr *header = NULL;
        const uint8_t *raw = NULL;

        int ret = pcap_next_ex(handle, &header, &raw);
        if (ret == 1) {
            packet_t packet(sizeof(udp_header_t));
            for (size_t i = 0; i < header->caplen; i++) {
                packet.push(raw[i]);
                if (packet.get_out() != 
                        packet_t::outcome::incomplete) {
                    break;
                }
            }
            if (packet.get_out() == packet_t::outcome::ok) {
                packets.push_back(packet);
                return true;
            }
        } 
        return false;
    }
};

}

#endif
