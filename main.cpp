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

        auto& data = back.data();
        if (!data.size())
            continue;
        const auto fields = data.back().get_fields();
        for (const auto& field : fields) {
            std::cout << field.first << std::endl;
        }

    }
    return 0;
}
