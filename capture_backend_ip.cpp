#include "capture_backend_ip.hpp"

namespace capture {

pcap_t *backend_ip_t::handle = nullptr;
char backend_ip_t::ebuf[PCAP_ERRBUF_SIZE] = {0};

backend_ip_t::backend_ip_t(const char *dev) {
    if (nrefs) {
        ok = true;
        nrefs++;
        return;
    }
    if (pcap_init(0, ebuf) != 0) {
        return;
    }  
    if (!(handle = pcap_create(dev, ebuf))) {
        return;
    }

	if (pcap_lookupnet(dev, &net, &mask, ebuf) == -1) {
        return;
    }

    pcap_set_snaplen(handle, 65535);
	pcap_set_promisc(handle, 1);
	pcap_set_immediate_mode(handle, 1);
    pcap_setnonblock(handle, 1, ebuf);

	if (pcap_activate(handle) != 0) {
        return;
    }

    ok = true;
    nrefs++;
}

}
