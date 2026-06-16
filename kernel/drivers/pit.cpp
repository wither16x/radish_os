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

bool sleeping = false;
bool consuming = false;

} /* anonymous namespace */

void init()
{
        u16 tps = 1000; // tics per second
        u16 divider = 1193181 / tps;

        cpu::output_byte_port(Port::Command, 0b00110100);
        cpu::output_byte_port(Port::Channel0, divider & 0xff);
        cpu::output_byte_port(Port::Channel0, (divider >> 8) & 0xff);
}

void sleep(lib::u64 ms)
{
        sleeping = true;
        while (tics < ms);
        sleeping = false;
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

bool is_sleeping()
{
        return sleeping;
}

bool consumed_tick()
{
        return consuming;
}

void consume_tick(bool state)
{
        consuming = state;
}

} /* namespace kernel::drivers::pit */