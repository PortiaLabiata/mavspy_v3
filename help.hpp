#ifndef HELP_H
#define HELP_H

#include <string>
std::string help_msg = "MAVSpy v3.0. Help:\n"
                       "--interface\tSelects network interface, defaults to local loop\n"
                       "--help, -h\tPrint this message and exit.\n"
                       "--print-socks\tPrint sockets that messages originate from\n"
                       "--print-fields\tPrint fields' values\n"
                       "--verbose, -v\tEquivalent to --print-socks --print-fields\n";


#endif
