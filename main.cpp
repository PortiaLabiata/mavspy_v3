#include <iostream>
#include <cstdlib>
#include "capture_backend_udp.hpp"
#include "gui_backend.hpp"
#include "ui_utility.hpp"

static void _draw_packet(const capture::packet_t& pkt) {
    auto hdr = reinterpret_cast<const capture::udp_header_t*>(
            pkt.get_data().data());

    auto& ip_hdr = hdr->ip.ip;
    auto& udp_hdr = hdr->udp;

    ImGui::TableNextColumn(); 
    ImGui::Text("%s", ui::print_socket(ip_hdr.saddr, udp_hdr.source)
            .c_str());

    ImGui::TableNextColumn(); 
    ImGui::Text("%s", ui::print_socket(ip_hdr.daddr, udp_hdr.dest)
            .c_str());
}

int main(int argc, char **argv) {
    capture::backend_udp_t back("lo");
    if (back.is_err()) {
        std::cout << "Could not init pcap" << std::endl;
        std::exit(127);
    } else {
        std::cout << "Inited pcap" << std::endl;
    }

    ui::gui_t gui(ImVec2(1366, 768));

    while (!gui.should_close()) {
        back.listen();
        gui.begin_frame();

        ImGui::Begin("Test", nullptr, 0); 
        ImGui::BeginTable("Messages", 2);
        for (const auto& pkt : back.data()) {
            _draw_packet(pkt);
        }
        ImGui::EndTable();
        ImGui::End();

        gui.end_frame();
    }
    return 0;
}
