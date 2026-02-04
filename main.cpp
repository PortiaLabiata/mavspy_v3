#include <iostream>
#include <cstdlib>
#include "capture_backend_udp.hpp"
#include "capture_backend_fs.hpp"
#include "capture_maker.hpp"
#include "ui_utility.hpp"

#include "help.hpp"
#include "argh.h"

struct args_t {
    std::string interface;
    std::string log_name;
    bool print_fields;
    bool print_socks;
    bool verbose;
};

args_t args;
int main(int argc, char **argv) {
    auto parser = argh::parser(argc, argv);

    args.interface = parser("interface", "lo").str();
    args.print_fields = parser["print-fields"];
    args.print_socks = parser["print-socks"];
    args.verbose = parser[{"-v", "--verbose"}];
    args.log_name = parser("log-file", "none").str();

    if (parser[{"-h", "--help"}]) {
        std::cout << help_msg;
        std::exit(0);
    }

    auto* back_main = capture::make_backend(
            capture::backend_type::udp, 
            args.interface.c_str());

    if (back_main->is_err()) {
        std::cout << "Could not init pcap" 
                  << std::endl;
        std::exit(127);
    } 

    args.print_socks |= args.verbose;
    args.print_fields |= args.verbose;

    while (1) {
        if (back_main->listen()) {
            auto& pkt = back_main->data().back();
            if (args.print_socks)
                std::cout << ui::print_packet_sock(pkt);

            std::cout << ui::print_packet_message(pkt);

            if (args.print_fields)
                std::cout << ui::print_packet_fields(pkt);
            std::cout << std::endl;
        }
    }
    return 0;
}
