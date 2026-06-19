#include <lib/logging.hpp>
#include <lib/typing.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>
#include <panic.hpp>

using kernel::lib::u8, kernel::lib::u64, kernel::lib::usize;

namespace kernel::proc {

void proc_init(Process *p, int id, void (*entry)())
{
        p->id = id;
        p->entry = entry;

        // prepare stack and registers
        u64 *stack_top = reinterpret_cast<u64 *>(
                (reinterpret_cast<u64>(p->stack) + PROCESS_STACK_SIZE)
        );
        // imagine the process started after irteq
        *--stack_top = 0x10;                                    // SS
        *--stack_top = reinterpret_cast<u64>(stack_top);        // RSP
        *--stack_top = (1 << 9);                                // flags
        *--stack_top = 0x08;                                    // CS
        *--stack_top = reinterpret_cast<u64>(entry);            // RIP
        p->rsp = reinterpret_cast<u64>(stack_top);
        p->rip = reinterpret_cast<u64>(entry);

        p->cs = 0x08;
        p->ss = 0x10;
        p->flags = (1 << 9);
}

} /* namespace kernel::proc */