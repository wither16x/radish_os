#include <drivers/pic.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>

using kernel::lib::u64;
using kernel::lib::log::logger;

namespace kernel::cpu {

namespace {

// registers pushed before calling `irq_handler()`
struct [[gnu::packed]] CPUFrame {
        u64 irqno;
	u64 cr2;
	u64 cr3;
	u64 rax;
	u64 rbx;
	u64 rcx;
	u64 rdx;
	u64 rsi;
	u64 rdi;
	u64 rbp;
	u64 r8;
	u64 r9;
	u64 r10;
	u64 r11;
	u64 r12;
	u64 r13;
	u64 r14;
	u64 r15;
	u64 rip;
	u64 cs;
	u64 flags;
	u64 rsp;
	u64 ss;
};

int irq0_tic = 0;
int irq0_sec = 0;

void handle_irq0_timer()
{
        irq0_tic++;

        if (irq0_tic % 1000 == 0) {
                irq0_tic = 0;
                irq0_sec++;
        }

        drivers::pic::send_eoi(0);
}

} /* anonymous namespace */

extern "C" void irq_handler(CPUFrame *f)
{
        switch (f->irqno) {
        case 0:
                handle_irq0_timer();
                break;

        default:
                logger.err("received unexpected IRQ: %u", f->irqno);
                break;
        }
}

} /* namespace kernel::cpu */