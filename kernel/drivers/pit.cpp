#include <cpu/io.hpp>
#include <drivers/pit.hpp>
#include <lib/typing.hpp>

using kernel::lib::u16, kernel::lib::u64;

namespace kernel::drivers::pit {

namespace {

enum Port : u16 {
        Command         = 0x43,
        Channel0        = 0x40
};

u64 tics = 0;
u64 seconds = 0;

} /* anonymous namespace */

void init()
{
        u16 tick_per_sec = 1000;
        u16 divider = 1193181 / tick_per_sec;

        cpu::output_byte_port(Port::Command, 0b00110100);
        cpu::output_byte_port(Port::Channel0, divider & 0xff);
        cpu::output_byte_port(Port::Channel0, (divider >> 8) & 0xff);
}

void sleep(lib::u64 ms)
{
        // no need for checks, remember that tics are
        // managed by the IRQ handler!
        u64 target_time = tics + ms;
        while (tics < target_time);
}

u64 get_tics()
{
        return tics;
}

u64 get_seconds()
{
        return seconds;
}

void set_tics(lib::u64 n)
{
        tics = n;
}

void set_seconds(lib::u64 n)
{
        seconds = n;
}

} /* namespace kernel::drivers::pit */