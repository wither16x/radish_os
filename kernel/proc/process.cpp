#include <kernel.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>
#include <mem/vmm.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>
#include <panic.hpp>

using kernel::lib::u8, kernel::lib::u64, kernel::lib::usize, kernel::lib::uptr;

namespace kernel::proc {

namespace {

int curr_pid = 0;

} /* anonymous namespace */

void proc_init(Process *p, int id, void (*entry)())
{
        p->id = id;
        p->entry = entry;

        // prepare stack and registers
        u64 *stack_top = reinterpret_cast<u64 *>(
                (reinterpret_cast<u64>(p->stack) + PROCESS_STACK_SIZE)
        );
        // do as if the process started after irteq
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

int allocate_pid()
{
        return curr_pid++;
}

void create_address_space(Process *p)
{
        u64 *kpml4t = get_kernel_pml4t();
        p->pml4t = mem::vmm::create_pml4t(kpml4t);
}

void load_address_space(Process *p)
{
        mem::vmm::load(p->pml4t);
}

} /* namespace kernel::proc */