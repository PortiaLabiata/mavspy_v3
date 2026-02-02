#ifndef CAPTURE_BACKEND_H
#define CAPTURE_BACKEND_H

#include <cstdint>
#include <vector>
#include <list>
#include "mavlink/all/mavlink.h"

namespace capture {

struct packet_t {
    packet_t(size_t _offset) :
        offset(_offset) {}
    ~packet_t() = default;

    enum class outcome {
        incomplete,
        ok,
        error
    };

    void push(uint8_t byte) {
        data.push_back(byte);
        if (data.size() < offset)
            return;

        int res = mavlink_parse_char(0, byte, 
                &msg, &status);
		if (res != MAVLINK_FRAMING_INCOMPLETE) {
            out = res == MAVLINK_FRAMING_OK ?
                outcome::ok : outcome::error;
        }
    }

    explicit operator mavlink_message_t() const {
        return msg;
    }

    outcome get_out() const { return out; }
private:
    std::vector<uint8_t> data;
    const size_t offset;

    mavlink_message_t msg;
    mavlink_status_t status;

    outcome out = outcome::incomplete;
};

class backend_t {
public:
    backend_t() = default;
    virtual ~backend_t() {};

    virtual void listen() = 0;

    const std::list<packet_t>& data() const {
        return packets;
    }
    bool is_ok() const { return ok; }
    bool is_err() const { return !ok; }
protected:
    bool ok = false;
    std::list<packet_t> packets;
};

}

#endif
