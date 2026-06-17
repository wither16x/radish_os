#include <drivers/pic.hpp>
#include <drivers/pit.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>

#include <lib/logging.hpp>
using kernel::lib::log::logger;

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

void handle_irq0_timer()
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

	if (!proc::scheduler::is_active()) {
		logger.debug("irq0: scheduler is not active");
		goto end;
	}

	if (proc::scheduler::proc_time_elapsed()) {
		logger.debug("irq0: process time elapsed, scheduling...");
		proc::scheduler::schedule();
	} else {
		logger.debug("irq0: process has time left");
		proc::scheduler::inc_proc_time();
	}
        
end:
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