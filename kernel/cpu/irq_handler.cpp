#include <cpu/irq_handler.hpp>
#include <drivers/pic.hpp>
#include <drivers/pit.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>

using kernel::lib::u64;
using kernel::lib::log::logger;

namespace kernel::cpu {

namespace {

void handle_irq0_timer(cpu::IRQFrame *f)
{
	u64 tics = drivers::pit::get_tics();
	u64 seconds = drivers::pit::get_seconds();

        drivers::pit::set_tics(tics + 1);

	// update tics since they have been modified above
	tics = drivers::pit::get_tics();

	// reset tics every second so the tic counter never
	// overflows
	// I will maybe add minutes, hours, days, and so on
        if (tics % 1000 == 0 && !drivers::pit::is_sleeping()) {
                drivers::pit::set_tics(0);
                drivers::pit::set_seconds(seconds + 1);
        }

        drivers::pic::send_eoi(0);

        if (!proc::scheduler::is_active())
                return;

        proc::scheduler::tick(f);
}

} /* anonymous namespace */

extern "C" void irq_handler(IRQFrame *f)
{
        switch (f->irqno) {
        case 0:
                handle_irq0_timer(f);
                break;

        default:
                logger.err("received unexpected IRQ: %u", f->irqno);
                break;
        }
}

} /* namespace kernel::cpu */