#ifndef CAPTURE_BACKEND_H
#define CAPTURE_BACKEND_H

#include <cstdint>
#include <vector>
#include <list>
#include <variant>
#include <utility>
#include <string>
#include <fstream>
#include <iostream>

extern "C" {

#define MAVLINK_USE_MESSAGE_INFO
#include "mavlink/all/mavlink.h"

}

namespace capture {

enum class backend_type {
    udp,
    tcp,
    serial,
    fs,
};

using mav_value_t = std::variant<int8_t, char, int16_t, int32_t, int64_t,
    float, double, 
    uint8_t, uint16_t, uint32_t, uint64_t>;
using mav_field_t = std::pair<std::string, mav_value_t>;

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

    void save(std::fstream& stream) {
        stream.write(reinterpret_cast<const char*>(data.data()), 
                data.size());
    }

    std::string get_name() const {
	    const mavlink_message_info_t *info = \
            mavlink_get_message_info(&msg);
        if (!info)
            return std::string("NO INFO");
        return info->name;
    }

    std::vector<mav_field_t> get_fields() const {
	    const mavlink_message_info_t *info = \
            mavlink_get_message_info(&msg);
        if (!info)
            return std::vector<mav_field_t>{0};

        std::vector<mav_field_t> res;
        res.reserve(info->num_fields);

	    for (size_t i = 0; i < info->num_fields; i++) {
		    const mavlink_field_info_t *field = &info->fields[i];
		    uint8_t *ptr = (uint8_t*)((uint8_t*)msg.payload64 + \
						(ptrdiff_t)field->wire_offset);
            mav_value_t value;
            switch (field->type) {
			    case MAVLINK_TYPE_INT8_T:
				    value =  *(int8_t*)(ptr);
				    break;
			    case MAVLINK_TYPE_CHAR:
				    value =  *(char*)(ptr);
				    break;
			    case MAVLINK_TYPE_INT16_T:
				    value =  *(int16_t*)(ptr);
				    break;
			    case MAVLINK_TYPE_INT32_T:
				    value =  *(int32_t*)(ptr);
				    break;
			    case MAVLINK_TYPE_INT64_T:
				    value = *(int64_t*)(ptr);
				    break;
			
		    	case MAVLINK_TYPE_FLOAT:
				    value = *(float*)(ptr);
				    break;
			    case MAVLINK_TYPE_DOUBLE:
				    value = *(double*)(ptr);
				    break;

			    case MAVLINK_TYPE_UINT8_T:
				    value =  *(uint8_t*)(ptr);
				    break;
			    case MAVLINK_TYPE_UINT16_T:
				    value =  *(uint16_t*)(ptr);
				    break;
			    case MAVLINK_TYPE_UINT32_T:
				    value =  *(uint32_t*)(ptr);
				    break;
			    case MAVLINK_TYPE_UINT64_T:
				    value = *(uint64_t*)(ptr);
				    break;
		    }
            res.emplace_back(std::make_pair(
                field->name,
                value
            ));
        }
        return res;
    }

    outcome get_out() const { return out; }
    std::vector<uint8_t>& get_data() { return data; }
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

    virtual bool listen() = 0;
    virtual bool should_exit() = 0;

    std::list<packet_t>& data() {
        return packets;
    }
    bool is_ok() const { return ok; }
    bool is_err() const { return !ok; }
protected:
    bool ok = false;
    std::list<packet_t> packets;
    backend_type type;

    // This stupid ass hack is necessary
    // for access to base class' protected members
    // from derived class
    static std::list<packet_t>& get_packets(backend_t *inst) {
        return inst->packets;
    }
};

}

#endif
