#pragma once

#include <cpu/assembly.hpp>
#include <lib/typing.hpp>

namespace kernel::cpu {

inline void output_byte_port(lib::u16 port, lib::u8 byte)
{
        outb(port, byte);
}

inline lib::u8 input_byte_port(lib::u16 port)
{
        return inb(port);
}

inline void io_wait()
{
        outb(0x80, 0);
}

} /* namespace kernel::cpu */