#ifndef CAPTURE_BACKEND_IP_H
#define CAPTURE_BACKEND_IP_H

extern "C" {

#include <pcap.h>
#include <linux/if_ether.h>
#include <netinet/ip.h>

}

#include "capture_backend.hpp"

namespace capture {

struct ip_header_t {
    ethhdr eth;
    iphdr ip;
} __attribute__((packed));

class backend_ip_t : public backend_t {
public:
    backend_ip_t(const char *dev);
    virtual ~backend_ip_t() {
        if (--nrefs == 0) {
            pcap_close(handle);
        }
    }
protected:
    static pcap_t *handle;
	bpf_u_int32 mask;
	bpf_u_int32 net; 
private:
    static char ebuf[PCAP_ERRBUF_SIZE];
    size_t nrefs = 0;
};

}

#endif
