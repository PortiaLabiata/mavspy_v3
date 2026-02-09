#ifndef HELP_H
#define HELP_H

#include <string>
std::string help_msg = "MAVSpy v3.0. Help:\n"
                       "--interface\tSelects network interface, defaults to local loop\n"
                       "--help, -h\tPrint this message and exit.\n"
                       "--print-socks, -s\tPrint sockets that messages originate from\n"
                       "--print-fields, -v\tPrint fields' values\n"
                       "--print-signature\tPrint packets' signatures\n"
                       "--print-dump\tPrint hex dump of packets\n"
                       "--print-crc\tPrint packet CRC\n"
                       "--verbose, -v\tEquivalent to --print-socks --print-fields\n"
                       "--to-file, -t\tSave data to binary file\n"
                       "--from-file, -f\tRead data from binary file\n"
                       "--time\tDuration of recording, set to 0 for unlimited\n";


#endif
