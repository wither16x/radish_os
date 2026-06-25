#pragma once

#include <cpu/assembly.hpp>
#include <lib/typing.hpp>

namespace kernel::cpu {

// NOTE: it is recommended to use the functions implemented
// below instead of using the assembly wrappers directly as
// they can wrap several assembly wrappers if needed.

/// Output a byte to a port.
inline void output_byte_port(lib::u16 port, lib::u8 byte)
{
        outb(port, byte);
}

/// Get a byte from a port.
inline lib::u8 input_byte_port(lib::u16 port)
{
        return inb(port);
}

/// Small delay.
inline void io_wait()
{
        outb(0x80, 0);
}

} /* namespace kernel::cpu */