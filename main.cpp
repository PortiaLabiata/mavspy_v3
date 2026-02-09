#include <iostream>
#include <cstdlib>
#include <chrono>

#include "capture_maker.hpp"
#include "ui_utility.hpp"

#include "help.hpp"
#include "argh.h"

struct args_t {
    std::string interface;
    std::string write_file;
    std::string read_file;

    bool print_fields;
    bool print_socks;
    bool print_sign;
    bool print_dump;
    bool print_crc;
    bool verbose;

    int timeout;
    size_t dump_step;
};

capture::backend_t *back_main = nullptr;
capture::backend_t *back_fs = nullptr;

args_t args;
int main(int argc, char **argv) {
    auto parser = argh::parser(argc, argv);

    args.interface = parser("interface", "lo").str();
    args.print_fields = parser[{"print-fields", "-v"}];
    args.print_socks = parser[{"print-socks", "-s"}];
    args.verbose = parser[{"-v", "--verbose"}];
    args.print_sign = parser["--print-signature"];
    args.print_dump = parser["--print-dump"];
    args.print_crc = parser["--print-crc"];
    args.timeout = -1;

    if (parser[{"-h", "--help"}]) {
        std::cout << help_msg;
        std::exit(0);
    }

    parser("dump-step", 16) >> args.dump_step;

    if (parser({"-f", "--from-file"}) >> args.read_file) {
        back_main = capture::make_backend(
                capture::backend_type::fs,
                args.read_file.c_str());
    } else {
        back_main = capture::make_backend(
                capture::backend_type::udp,
                args.interface.c_str());
    }

    if (parser({"-t", "--to-file"}) >> args.write_file) {
        back_fs = capture::make_backend(
                capture::backend_type::fs,
                args.write_file.c_str());
    }

    if (!(parser("--time") >> args.timeout) && 
            back_fs) {
        std::cout << "Cannot write to file without timeout"
                  << std::endl;
        std::exit(127);
    }

    if (back_main->is_err()) {
        std::cout << "Could not init capture backend" 
                  << std::endl;
        std::exit(127);
    } 

    args.print_socks |= args.verbose;
    args.print_fields |= args.verbose;

    auto start_time = std::chrono::steady_clock::now();
    auto end_time = start_time + 
        std::chrono::milliseconds(args.timeout);
    
    while (!back_main->should_exit()) {
        if (args.timeout > 0 && 
                std::chrono::steady_clock::now() >= end_time) {
            break;
        }
        if (back_main->listen()) {
            auto& pkt = back_main->data().back();

            if (args.print_socks)
                std::cout << ui::print_packet_sock(pkt);

            if (args.print_crc)
                std::cout << ui::print_packet_crc(pkt);

            std::cout << ui::print_packet_message(pkt);

            if (args.print_sign)
                std::cout << ui::print_packet_signature(pkt);

            if (args.print_fields)
                std::cout << ui::print_packet_fields(pkt);
           
            std::cout << std::endl;

            if (args.print_dump)
                std::cout << ui::print_packet_dump(pkt, args.dump_step)
                          << std::endl;
        }
    }

    if (back_fs && back_main) {
        auto *back = dynamic_cast<
            capture::backend_fs_t*
        >(back_fs);

        back->save(back_main);
        std::cout << "Saved data to file" << std::endl;
    }
    return 0;
}
