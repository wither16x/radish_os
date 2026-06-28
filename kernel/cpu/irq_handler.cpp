#include <lib/print.hpp>
#include <cpu/io.hpp>
#include <cpu/irq_handler.hpp>
#include <drivers/keyboard.hpp>
#include <drivers/pic.hpp>
#include <drivers/pit.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>

using kernel::lib::u8, kernel::lib::u64;
using kernel::lib::log::logger;

namespace kernel::cpu {

namespace {

/// Handle the timer IRQ.
void handle_irq0_timer(cpu::IRQFrame *f)
{
        // increase time and consider that the interrupt is
        // finished
        drivers::pit::tick();
        drivers::pic::send_eoi(drivers::pic::IRQ_TIMER);

        // schedule
        if (!proc::scheduler::is_active())
                return;
        proc::scheduler::tick(f);
}

/// Handle the keyboard IRQ.
void handle_irq1_keyboard()
{
        u8 k = drivers::keyboard::handle_key_press();
        char ch = drivers::keyboard::scancode_to_key(k);
        if (ch)
                lib::putchar(ch);

        drivers::pic::send_eoi(drivers::pic::IRQ_KEYBOARD);
}

} /* anonymous namespace */

extern "C" void irq_handler(IRQFrame *f)
{
        switch (f->irqno) {
        case drivers::pic::IRQ_TIMER:
                handle_irq0_timer(f);
                break;

        case drivers::pic::IRQ_KEYBOARD:
                handle_irq1_keyboard();
                break;

        default:
                logger.err("received unexpected IRQ: %u", f->irqno);
                break;
        }
}

} /* namespace kernel::cpu */