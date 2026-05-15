#pragma once

#include <lib/status.hpp>
#include <lib/typing.hpp>

namespace kernel {

namespace drivers {

namespace serial {

enum class Port : lib::u16 {
        COM1            = 0x3f8,
        COM2            = 0x2f8
        // The other COM ports are not specified here as their address
        // is less reliable, and we also do not need them. We do not
        // even need COM2 actually.
};

lib::Status init_port(Port port);
void send_byte(Port port, lib::u8 byte);

} /* namespace serial */

} /* namespace drivers */

} /* namespace drivers */