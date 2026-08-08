#include <lib/print.hpp>
#include <cpu/io.hpp>
#include <cpu/irq.hpp>
#include <drivers/keyboard.hpp>
#include <drivers/pic.hpp>
#include <drivers/pit.hpp>
#include <kernel.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>

using kernel::lib::u8, kernel::lib::u64;
using kernel::lib::log::logger;

namespace kernel::cpu {

namespace {

constexpr int MAX_IRQ_HANDLERS = 16;

void (*handlers[MAX_IRQ_HANDLERS])(IRQFrame &);

} /* anonymous namespace */

// --------------------------------------------------
void register_irq(int n, void (*handler)(IRQFrame &))
{
        handlers[n] = handler;
        drivers::pic::irq_unmask(n);
}
// --------------------------------------------------

// --------------------------------------------------
extern "C" void irq_handler(IRQFrame &f)
{
        if (f.irqno >= MAX_IRQ_HANDLERS) {
                logger.err("received unexpected irq: %u", f.irqno);
                return;
        }

        void (*handler)(IRQFrame &) = handlers[f.irqno];
        if (not handler) {
                logger.err("no handler available for irq %u", f.irqno);
                return;
        }
        
        handler(f);
}
// --------------------------------------------------

} /* namespace kernel::cpu */