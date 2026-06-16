#include <lib/typing.hpp>
#include <lib/time.hpp>
#include <proc/process.hpp>
#include <panic.hpp>

using kernel::lib::u8, kernel::lib::u64, kernel::lib::usize;
using kernel::lib::Timer;

namespace kernel::proc {

namespace {

Process *trampoline_process = nullptr;

void trampoline()
{
        Process *p = trampoline_process;
        p->entry();
        p->exit();
        panic("process exited unexpectedly");
}

} /* anonymous namespace */

void proc_init(Process *p, int id, void (*entry)(), void (*exit)())
{
        p->entry        = entry;
        p->exit         = exit;

        p->id           = id;

        p->state        = ProcessState::Idle;
        p->timer        = Timer(0, TIME_PER_PROCESS, 1);

        p->stack        = new u8[PROCESS_STACK_SIZE];

        u8 *stack_top = p->stack + PROCESS_STACK_SIZE;
        stack_top -= reinterpret_cast<usize>(stack_top) % 16;

        auto push = [&stack_top](u64 val) -> void {
                stack_top -= sizeof(u64);
                *reinterpret_cast<u64 *>(stack_top) = val;
        };

	push(reinterpret_cast<lib::u64>(&trampoline));
	push(0); // cr3
	push(0); // cr2
	push(0); // rax
	push(0); // rbx
	push(0); // rcx
	push(0); // rdx
	push(reinterpret_cast<lib::u64>(p)); // rdi
	push(0); // rsi
	push(0); // r8
	push(0); // r9
	push(0); // r10
	push(0); // r11
	push(0); // r12
	push(0); // r13
	push(0); // r14
	push(0); // r15
	push(0); // rbp

        p->rsp = stack_top;
}

} /* namespace kernel::proc */