#include <lib/typing.hpp>
#include <proc/process.hpp>
#include <panic.hpp>

using kernel::lib::u64, kernel::lib::uptr;

namespace kernel::proc {

void proc_exit()
{
        // no other process, hang forever
        panic("process exited");
}

void proc_init(Process *p, void (*entry)())
{
        // stack top (16-bit aligned)
        u64 *sp = reinterpret_cast<u64 *>(
                reinterpret_cast<uptr>(p->stack) + sizeof(p->stack)
        );

        // push the exit function
        *--sp = reinterpret_cast<u64>(proc_exit);
        // push the entry point
        *--sp = reinterpret_cast<u64>(entry);

        // set all callee-saved registers to 0
        for (int i = 0; i < 6; i++)
                *--sp = 0;

        // set new stack pointer
        p->rsp = sp;
}

} /* namespace kernel::proc */