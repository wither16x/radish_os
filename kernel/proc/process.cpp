#include <cpu/irq.hpp>
#include <cpu/userspace.hpp>
#include <lib/typing.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <proc/process.hpp>

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
        this->rsp = cpu::USER_STACK_TOP;

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

void Process::save_context(this Process &self, cpu::IRQFrame *frame)
{
        self.rax   = frame->rax;
        self.rbx   = frame->rbx;
        self.rcx   = frame->rcx;
        self.rdx   = frame->rdx;
        self.rsi   = frame->rsi;
        self.rdi   = frame->rdi;
        self.rbp   = frame->rbp;
        self.r8    = frame->r8;
        self.r9    = frame->r9;
        self.r10   = frame->r10;
        self.r11   = frame->r11;
        self.r12   = frame->r12;
        self.r13   = frame->r13;
        self.r14   = frame->r14;
        self.r15   = frame->r15;
        self.rip   = frame->rip;
        self.cs    = frame->cs;
        self.flags = frame->flags | (1 << 9);
        self.rsp   = frame->rsp;
        self.ss    = frame->ss;
        self.cr2   = frame->cr2;
        self.cr3   = frame->cr3;
}

void Process::load_context(this Process &self, cpu::IRQFrame *frame)
{
        frame->rax      = self.rax;
        frame->rbx      = self.rbx;
        frame->rcx      = self.rcx;
        frame->rdx      = self.rdx;
        frame->rsi      = self.rsi;
        frame->rdi      = self.rdi;
        frame->rbp      = self.rbp;
        frame->r8       = self.r8;
        frame->r9       = self.r9;
        frame->r10      = self.r10;
        frame->r11      = self.r11;
        frame->r12      = self.r12;
        frame->r13      = self.r13;
        frame->r14      = self.r14;
        frame->r15      = self.r15;
        frame->rip      = self.rip;
        frame->cs       = self.cs;
        frame->flags    = self.flags | (1 << 9);
        frame->rsp      = self.rsp;
        frame->ss       = self.ss;
        frame->cr2      = self.cr2;
        frame->cr3      = self.cr3;
}

// --------------------------------------------------
int allocate_pid()
{
        return curr_pid++;
}
// --------------------------------------------------

} /* namespace kernel::proc */