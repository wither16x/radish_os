#include <cpu/io.hpp>
#include <drivers/pit.hpp>
#include <lib/typing.hpp>

using kernel::lib::u16, kernel::lib::u64;

namespace kernel::drivers::pit {

namespace {

/// Enumeration of the PIT ports.
enum Port : u16 {
        PORT_COMMAND         = 0x43,
        PORT_CHANNEL_0       = 0x40
};

u64 tics = 0;
u64 seconds = 0;

bool sleeping = false;

} /* anonymous namespace */

// --------------------------------------------------
void init()
{
        u16 tps = 1000; // tics per second
        u16 divider = 1193181 / tps;

        cpu::output_byte_port(Port::PORT_COMMAND, 0b00110100);
        cpu::output_byte_port(Port::PORT_CHANNEL_0, divider & 0xff);
        cpu::output_byte_port(Port::PORT_CHANNEL_0, (divider >> 8) & 0xff);
}
// --------------------------------------------------

// --------------------------------------------------
void tick()
{
        tics++;

	// reset tics every second so the tic counter never
	// overflows
	// I will maybe add minutes, hours, days, and so on
        if (tics % 1000 == 0 && !sleeping) {
                tics = 0;
                seconds++;
        }
}
// --------------------------------------------------

// --------------------------------------------------
void sleep(lib::u64 ms)
{
        sleeping = true;
        while (tics < ms);
        sleeping = false;
}
// --------------------------------------------------

// --------------------------------------------------
u64 get_tics()
{
        return tics;
}
// --------------------------------------------------

// --------------------------------------------------
u64 get_seconds()
{
        return seconds;
}
// --------------------------------------------------

} /* namespace kernel::drivers::pit */