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

/// Handle the timer IRQ.
void handle_irq0_timer(cpu::IRQFrame *f)
{
        // increase time and consider that the interrupt is
        // finished
        drivers::pit::tick();
        drivers::pic::send_eoi(0);

        // schedule
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