#pragma once

#include <lib/status.hpp>
#include <lib/typing.hpp>

namespace kernel::drivers::serial {

enum Port : lib::u16 {
        SERIAL_COM1            = 0x3f8,
        SERIAL_COM2            = 0x2f8
        // The other COM ports are not specified here as their address
        // is less reliable, and we also do not need them. We do not
        // even need COM2 actually.
};

lib::Status init_port(lib::u16 port);
void send_byte(lib::u16 port, lib::u8 byte);

} /* namespace kernel::drivers::serial */