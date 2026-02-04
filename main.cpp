#include <iostream>
#include <cstdlib>
#include "capture_backend_udp.hpp"
#include "ui_utility.hpp"

#include "help.hpp"
#include "argh.h"

int main(int argc, char **argv) {
    auto parser = argh::parser(argc, argv);

    capture::backend_udp_t back(
        parser("interface", "lo").str().c_str()
    );
    
    if (parser[{"-h", "--help"}]) {
        std::cout << help_msg;
        std::exit(0);
    }
    bool print_fields = parser["print-fields"];
    bool print_socks = parser["print-socks"];

    if (back.is_err()) {
        std::cout << "Could not init pcap" 
                  << std::endl;
        std::exit(127);
    } 

    while (1) {
        if (back.listen()) {
            auto& pkt = back.data().back();
            if (print_socks)
                std::cout << ui::print_packet_sock(pkt);

            std::cout << ui::print_packet_message(pkt);

            if (print_fields)
                std::cout << ui::print_packet_fields(pkt);
            std::cout << std::endl;
        }
    }
    return 0;
}
