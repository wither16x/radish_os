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
        drivers::pit::tick();
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