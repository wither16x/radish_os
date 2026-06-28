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

void *handlers[MAX_IRQ_HANDLERS];

} /* anonymous namespace */

// --------------------------------------------------
void register_irq(int n, void *handler)
{
        handlers[n] = reinterpret_cast<void *>(handler);
        drivers::pic::irq_unmask(n);
}
// --------------------------------------------------

// --------------------------------------------------
extern "C" void irq_handler(IRQFrame *f)
{
        if (f->irqno >= MAX_IRQ_HANDLERS) {
                logger.err("received unexpected irq: %u", f->irqno);
                return;
        }

        void *handler = handlers[f->irqno];
        if (!handler) {
                logger.err("handler for irq %u is null", f->irqno);
                return;
        }

        switch (f->irqno) {
        case drivers::pic::IRQ_TIMER: {
                void (*timer_handler)(IRQFrame *f) = reinterpret_cast<void (*)(IRQFrame *f)>(handler);
                timer_handler(f);
                break;
        }

        case drivers::pic::IRQ_KEYBOARD: {
                void (*keyboard_handler)() = reinterpret_cast<void (*)()>(handler);
                keyboard_handler();
                break;
        }

        default:
                logger.err("received unexpected irq: %u", f->irqno);
                break;
        }
}
// --------------------------------------------------

} /* namespace kernel::cpu */