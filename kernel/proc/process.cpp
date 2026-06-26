#include <cpu/userspace.hpp>
#include <kernel.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>
#include <panic.hpp>

using kernel::lib::u8, kernel::lib::u64, kernel::lib::usize, kernel::lib::uptr;

namespace kernel::proc {

namespace {

int curr_pid = 0;

} /* anonymous namespace */

// --------------------------------------------------
Process::Process(int id, void (*entry)(), lib::u64 *pml4t)
{
        this->id = id;
        this->entry = entry;

        for (uptr addr = cpu::USER_STACK_BOTTOM; addr < cpu::USER_STACK_TOP; addr += mem::vmm::PAGE_BYTES) {
                mem::vmm::map_page(pml4t,
                        addr,
                        mem::pmm::allocate_frame(),
                        mem::vmm::PageFlag::ReadWriteUser | mem::vmm::PageFlag::NoExec
                );
        }

        this->rip = reinterpret_cast<u64>(entry);
        this->cs = 0x1b;
        this->ss = 0x23;
        this->flags = 1 << 9;
        this->rsp = cpu::USER_STACK_TOP - 8;

        this->rax = this->rbx = this->rcx = this->rdx = 0;
        this->rsi = this->rdi = this->rbp = 0;
        this->r8  = this->r9  = this->r10 = this->r11 = 0;
        this->r12 = this->r13 = this->r14 = this->r15 = 0;
        this->cr2 = 0;
        this->cr3 = reinterpret_cast<u64>(pml4t);

        this->pml4t = pml4t;
}
// --------------------------------------------------

// --------------------------------------------------
void Process::load(this Process &self)
{
        mem::vmm::load(self.pml4t);
}
// --------------------------------------------------

// --------------------------------------------------
int allocate_pid()
{
        return curr_pid++;
}
// --------------------------------------------------

} /* namespace kernel::proc */