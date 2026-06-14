#include <cpu/io.hpp>
#include <drivers/pit.hpp>
#include <lib/typing.hpp>

using kernel::lib::u16;

namespace kernel::drivers::pit {

namespace {

enum Port : u16 {
        Command         = 0x43,
        Channel0        = 0x40
};

} /* anonymous namespace */

void init()
{
        u16 tick_per_sec = 1000;
        u16 divider = 1193181 / tick_per_sec;

        cpu::output_byte_port(Port::Command, 0b00110100);
        cpu::output_byte_port(Port::Channel0, divider & 0xff);
        cpu::output_byte_port(Port::Channel0, (divider >> 8) & 0xff);
}

} /* namespace kernel::drivers::pit */