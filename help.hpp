#ifndef HELP_H
#define HELP_H

#include <string>
std::string help_msg = "MAVSpy v3.0. Help:\n"
                       "--interface\tSelects network interface, defaults to local loop\n"
                       "--help, -h\tPrint this message and exit.\n"
                       "--verbose, -v\tEquivalent to --print-socks --print-fields\n"
                       "--to-file, -t\tSave data to binary file\n"
                       "--from-file, -f\tRead data from binary file\n"
                       "--time\tDuration of recording, set to 0 for unlimited\n"
                       "--format, -f\tFormat message output, set as follows:\n"
                       "\t%s - source and destination sockets;\n"
                       "\t%f - packet fields;\n"
                       "\t%m - equivalent to %y%o%M%s%n;\n"
                       "\t%S - message signature;\n"
                       "\t%c - message CRC;\n"
                       "\t%y - system ID;\n"
                       "\t%o - component ID;\n"
                       "\t%M - message ID;\n"
                       "\t%n - message name;\n"
                       "\t%e - SEQ.\n";


#endif
