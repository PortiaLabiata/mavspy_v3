#include <iostream>
#include <cstdlib>
#include "capture_backend_udp.hpp"
#include "ui_utility.hpp"

int main(int argc, char **argv) {
    capture::backend_udp_t back("lo");
    if (back.is_err()) {
        std::cout << "Could not init pcap" 
                  << std::endl;
        std::exit(127);
    } 

    while (1) {
        if (back.listen()) {
            auto& pkt = back.data().back();
            std::cout << ui::print_packet_sock(pkt)
                      << ui::print_packet_message(pkt)
                      << ui::print_packet_fields(pkt)
                      << std::endl;
        }
    }
    return 0;
}
