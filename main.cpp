#include <iostream>
#include <cstdlib>
#include "capture_backend_udp.hpp"

int main(int argc, char **argv) {
    capture::backend_udp_t back("lo");
    if (back.is_err()) {
        std::cout << "Could not init pcap" << std::endl;
        std::exit(127);
    } else {
        std::cout << "Inited pcap" << std::endl;
    }

    while (1) {
        back.listen();
    }
    return 0;
}
