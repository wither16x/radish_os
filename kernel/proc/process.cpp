#include <kernel.hpp>
#include <mem/pml4t.hpp>
#include <cpu/irq.hpp>
#include <cpu/userspace.hpp>
#include <cpu/assembly.hpp>
#include <lib/typing.hpp>
#include <lib/memory.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <mem/page.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>

using kernel::lib::u8, kernel::lib::u64, kernel::lib::usize, kernel::lib::uptr;
using kernel::lib::memset, kernel::lib::memcpy;

namespace kernel::proc {

namespace {

int curr_pid = 1;

} /* anonymous namespace */

void Process::init_kernel_stack(this Process &self)
{
        uptr hhdm_offset = get_kernel_hhdm_offset();

        self.kstack_frame = mem::pmm::allocate_frame();
        self.kstack_top = self.kstack_frame + hhdm_offset + mem::PAGE_SIZE;

        uptr *sp = reinterpret_cast<uptr *>(self.kstack_top);

        sp -= sizeof(ProcessStackFrame) / sizeof(u64);
        self.frame = reinterpret_cast<ProcessStackFrame *>(sp);
        memset(self.frame, 0, sizeof(ProcessStackFrame));
        self.frame->cs = 0x1b;
        self.frame->ss = 0x23;
        self.frame->flags = 1 << 9;

        *(--sp) = reinterpret_cast<u64>(&proc_trampoline);

        for (int i = 0; i < 6; i++)
                *(--sp) = 0;

        self.krsp = reinterpret_cast<uptr>(sp);
}

// --------------------------------------------------
Process::Process(int id, void (*entry)(), mem::PML4T &pml4t)
{
        this->id = id;
        this->entry = entry;

        for (uptr addr = cpu::USER_STACK_BOTTOM; addr < cpu::USER_STACK_TOP; addr += mem::PAGE_SIZE) {
                pml4t.map_page(addr,
                        mem::pmm::allocate_frame(),
                        mem::PageFlag::ReadWriteUser | mem::PageFlag::NoExec
                );
        }
        this->pml4t = pml4t;
        this->status = ProcessStatus::Alive;

        this->init_kernel_stack();
        this->frame->rip = reinterpret_cast<u64>(entry);
        this->frame->rsp = cpu::USER_STACK_TOP;
}
// --------------------------------------------------

Process::Process(int id, const Process &parent, mem::PML4T &pml4t)
{
        uptr hhdm_offset = get_kernel_hhdm_offset();

        this->id = id;
        this->pml4t = pml4t;
        this->cr3 = reinterpret_cast<u64>(pml4t.raw()) - hhdm_offset;

        this->init_kernel_stack();
        memcpy(this->frame, parent.frame, sizeof(ProcessStackFrame));

        this->status = ProcessStatus::Alive;
}

// --------------------------------------------------
void Process::load(this Process &self)
{
        self.pml4t.load();
}
// --------------------------------------------------

void Process::switch_pml4t(this Process &self, const mem::PML4T &pml4t)
{
        self.pml4t.destroy();
        self.pml4t = pml4t;
}

void Process::save_context(this Process &self, cpu::IRQFrame *frame)
{
        self.frame->rax   = frame->rax;
        self.frame->rbx   = frame->rbx;
        self.frame->rcx   = frame->rcx;
        self.frame->rdx   = frame->rdx;
        self.frame->rsi   = frame->rsi;
        self.frame->rdi   = frame->rdi;
        self.frame->rbp   = frame->rbp;
        self.frame->r8    = frame->r8;
        self.frame->r9    = frame->r9;
        self.frame->r10   = frame->r10;
        self.frame->r11   = frame->r11;
        self.frame->r12   = frame->r12;
        self.frame->r13   = frame->r13;
        self.frame->r14   = frame->r14;
        self.frame->r15   = frame->r15;
        self.frame->rip   = frame->rip;
        self.frame->cs    = frame->cs;
        self.frame->flags = frame->flags | (1 << 9);
        self.frame->rsp   = frame->rsp;
        self.frame->ss    = frame->ss;
        self.cr3   = frame->cr3;
}

void Process::load_context(this Process &self, cpu::IRQFrame *frame)
{
        frame->rax      = self.frame->rax;
        frame->rbx      = self.frame->rbx;
        frame->rcx      = self.frame->rcx;
        frame->rdx      = self.frame->rdx;
        frame->rsi      = self.frame->rsi;
        frame->rdi      = self.frame->rdi;
        frame->rbp      = self.frame->rbp;
        frame->r8       = self.frame->r8;
        frame->r9       = self.frame->r9;
        frame->r10      = self.frame->r10;
        frame->r11      = self.frame->r11;
        frame->r12      = self.frame->r12;
        frame->r13      = self.frame->r13;
        frame->r14      = self.frame->r14;
        frame->r15      = self.frame->r15;
        frame->rip      = self.frame->rip;
        frame->cs       = self.frame->cs;
        frame->flags    = self.frame->flags | (1 << 9);
        frame->rsp      = self.frame->rsp;
        frame->ss       = self.frame->ss;
        frame->cr3      = self.cr3;
}

void Process::remap_stack(this Process &self)
{
        for (uptr addr = cpu::USER_STACK_BOTTOM; addr < cpu::USER_STACK_TOP; addr += mem::PAGE_SIZE) {
                self.pml4t.map_page(addr,
                        mem::pmm::allocate_frame(),
                        mem::PageFlag::ReadWriteUser | mem::PageFlag::NoExec
                );
        }
}

void Process::reset_stack(this Process &self)
{
        self.frame->rsp = cpu::USER_STACK_TOP;
}

void Process::switch_entry(this Process &self, void (*entry)())
{
        self.entry = entry;
        self.frame->rip = reinterpret_cast<u64>(entry);
}

int Process::add_child(this Process &self, Process *child)
{
        for (auto &proc : self.children) {
                if (proc->id == child->id)
                        return -1; // already has a child with same ID, probably itself
        }

        self.children.push_back(child);

        return 0;
}

int Process::remove_child(this Process &self, int id)
{
        for (usize i = 0; i < self.children.size(); i++) {
                if (self.children[i]->id == id) {
                        self.children.erase(i);
                        return 0;
                }
        }

        return -1; // no child found with this ID
}

// --------------------------------------------------
int allocate_pid()
{
        return curr_pid++;
}
// --------------------------------------------------

} /* namespace kernel::proc */