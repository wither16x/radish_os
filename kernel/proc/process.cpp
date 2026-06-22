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

Process::Process(int id, void (*entry)(), lib::u64 *pml4t)
{
        this->id = id;
        this->entry = entry;

        // prepare stack and registers
        u64 *stack_top = reinterpret_cast<u64 *>(
                (reinterpret_cast<u64>(this->stack) + PROCESS_STACK_SIZE)
        );
        // do as if the process started after irteq
        *--stack_top = 0x10;                                    // SS
        *--stack_top = reinterpret_cast<u64>(stack_top);        // RSP
        *--stack_top = (1 << 9);                                // flags
        *--stack_top = 0x08;                                    // CS
        *--stack_top = reinterpret_cast<u64>(entry);            // RIP
        this->rsp = reinterpret_cast<u64>(stack_top);
        this->rip = reinterpret_cast<u64>(entry);

        this->cs = 0x08;
        this->ss = 0x10;
        this->flags = (1 << 9);

        this->pml4t = pml4t;
}

void Process::load(this Process &self)
{
        mem::vmm::load(self.pml4t);
}

int allocate_pid()
{
        return curr_pid++;
}

} /* namespace kernel::proc */