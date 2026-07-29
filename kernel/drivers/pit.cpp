#include <cpu/io.hpp>
#include <cpu/irq.hpp>
#include <drivers/pic.hpp>
#include <drivers/pit.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>
#include <proc/scheduler.hpp>

using kernel::lib::u16, kernel::lib::u64;
using kernel::lib::log::logger;

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

void handle_irq(cpu::IRQFrame *frame)
{
        (void)frame;

        // increase time and consider that the interrupt is
        // finished
        tick();
        drivers::pic::send_eoi(drivers::pic::IRQ_TIMER);

        // schedule
        if (!proc::scheduler::is_active())
                return;
        proc::scheduler::tick();
}

} /* anonymous namespace */

// --------------------------------------------------
void init()
{
        u16 tps = 1000; // tics per second
        u16 divider = 1193181 / tps;

        cpu::output_byte_port(Port::PORT_COMMAND, 0b00110100);
        cpu::output_byte_port(Port::PORT_CHANNEL_0, divider & 0xff);
        cpu::output_byte_port(Port::PORT_CHANNEL_0, (divider >> 8) & 0xff);

        cpu::register_irq(drivers::pic::IRQType::IRQ_TIMER, handle_irq);

        logger.ok("initialized pit driver");
}
// --------------------------------------------------

// --------------------------------------------------
void tick()
{
        tics++;

	// reset tics every second so the tic counter never
	// overflows
	// I will maybe add minutes, hours, days, and so on
        if (tics % 1000 == 0 and not sleeping) {
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