#include <lib/typing.hpp>
#include <proc/process.hpp>
#include <panic.hpp>

using kernel::lib::u8, kernel::lib::u64, kernel::lib::usize;

namespace kernel::proc {

void proc_init(Process *p, int id, void (*entry)(), void (*exit)())
{
        p->entry        = entry;
        p->exit         = exit;
        p->id           = id;
        p->state        = ProcessState::Idle;
        p->stack        = new u8[PROCESS_STACK_SIZE];
	p->time		= 0;

        u8 *stack_top = p->stack + PROCESS_STACK_SIZE;
	// align 16 bits
        stack_top -= reinterpret_cast<usize>(stack_top) % 16;

        *--stack_top = reinterpret_cast<u64>(p->exit);
	*--stack_top = reinterpret_cast<u64>(p->entry);
        for (int i = 0; i < 17; i++)
                *--stack_top = 0;

        p->rsp = stack_top;
}

} /* namespace kernel::proc */