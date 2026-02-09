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

    std::string format; 
    bool verbose;
    bool print_dump;

    int timeout;
    size_t dump_step;
};

struct format_entry_t {
    char character;
    std::string (*func)(capture::packet_t&);
};

enum class format_state_t {
    regular,
    special,
    screened,
};

format_entry_t format_lookup[] = {
    {'s', ui::print_packet_sock},
    {'f', ui::print_packet_fields},
    {'m', ui::print_packet_message},
    {'S', ui::print_packet_signature},
    {'c', ui::print_packet_crc},

    {'y', ui::print_packet_sysid},
    {'o', ui::print_packet_compid},
    {'M', ui::print_packet_msgid},
    {'n', ui::print_packet_name},
    {'e', ui::print_packet_seq},
};
const size_t lookup_size = sizeof(format_lookup)/sizeof(format_entry_t);

static std::string _push_format(char c, capture::packet_t& pkt) {
    for (size_t i = 0; i < lookup_size; i++) {
        const auto& entry = format_lookup[i];
        if (c == entry.character)
            return entry.func(pkt);
    }
    return "";
}

args_t args;
static std::string _format_packet(capture::packet_t& pkt) {
    format_state_t state = format_state_t::regular;
    std::string res;

    for (auto c : args.format) {
        switch (state) {
            case format_state_t::regular: {
                if (c == '%') {
                    state = format_state_t::special;
                } else if (c == '\\') {
                    state = format_state_t::screened;
                } else {
                    res += c;
                }
            }
            break;
            case format_state_t::special: {
                res += _push_format(c, pkt);
                state = format_state_t::regular;
            }
            break;
            case format_state_t::screened: {
                res += c;
                state = format_state_t::regular;
            }
            break;
        }
    }    
    return res;
}

int main(int argc, char **argv) {
    capture::backend_t *back_main = nullptr;
    capture::backend_t *back_fs = nullptr;

    auto parser = argh::parser(argc, argv);

    args.verbose = parser[{"-v", "--verbose"}];
    args.print_dump = parser[{"-d", "--dump"}];

    parser("interface", "lo") >> args.interface;
    parser({"-f", "--format"}, "%m") >> args.format;
    parser("dump-step", 16) >> args.dump_step;

    if (args.verbose) {
        args.format = "%s%m%f";
    }

    args.timeout = -1;

    if (parser[{"-h", "--help"}]) {
        std::cout << help_msg;
        std::exit(0);
    }

    if (parser("--from-file") >> args.read_file) {
        back_main = capture::make_backend(
                capture::backend_type::fs,
                args.read_file.c_str());
    } else {
        back_main = capture::make_backend(
                capture::backend_type::udp,
                args.interface.c_str());
    }

    if (parser("--to-file") >> args.write_file) {
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
            std::cout << _format_packet(pkt)
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
