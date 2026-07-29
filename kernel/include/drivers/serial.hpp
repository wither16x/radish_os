#pragma once

#include <lib/typing.hpp>

namespace kernel::drivers::serial {

/// Enumeration of the serial ports.
/// The other COM ports are not specified here as their address
/// is less reliable, and we also do not need them. We do not
/// even need COM2 actually.
enum Port : lib::u16 {
        SERIAL_COM1            = 0x3f8,
        SERIAL_COM2            = 0x2f8
};

/// Initialize a serial port.
bool init_port(lib::u16 port);
/// Send a byte to a serial port.
void send_byte(lib::u16 port, lib::u8 byte);

} /* namespace kernel::drivers::serial */