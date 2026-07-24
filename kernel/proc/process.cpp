#include <kernel.hpp>
#include <mem/pml4t.hpp>
#include <cpu/irq.hpp>
#include <cpu/syscall.hpp>
#include <cpu/userspace.hpp>
#include <cpu/assembly.hpp>
#include <lib/typing.hpp>
#include <lib/memory.hpp>
#include <lib/vector.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <mem/page.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>

using kernel::lib::u8, kernel::lib::u64, kernel::lib::usize, kernel::lib::uptr;
using kernel::lib::memset, kernel::lib::memcpy;
using kernel::lib::Vector;

namespace kernel::proc {

void Process::init_kernel_stack(this Process &self)
{
        uptr hhdm_offset = get_kernel_hhdm_offset();

        self.kstack_frame = mem::pmm::allocate_frame();
        self.kstack_top = self.kstack_frame + hhdm_offset + mem::PAGE_SIZE;

        uptr *sp = reinterpret_cast<uptr *>(self.kstack_top);

        *(--sp) = self.frame->ss;
        *(--sp) = self.frame->rsp;
        *(--sp) = self.frame->flags;
        *(--sp) = self.frame->cs;
        *(--sp) = self.frame->rip;

        *(--sp) = reinterpret_cast<u64>(&proc_trampoline);

        *(--sp) = self.frame->rax;
        *(--sp) = self.frame->rbx;
        *(--sp) = self.frame->rcx;
        *(--sp) = self.frame->rdx;
        *(--sp) = self.frame->rsi;
        *(--sp) = self.frame->rdi;
        *(--sp) = self.frame->rbp;
        *(--sp) = self.frame->r8;
        *(--sp) = self.frame->r9;
        *(--sp) = self.frame->r10;
        *(--sp) = self.frame->r11;
        *(--sp) = self.frame->r12;
        *(--sp) = self.frame->r13;
        *(--sp) = self.frame->r14;
        *(--sp) = self.frame->r15;

        self.krsp = reinterpret_cast<uptr>(sp);
}

// --------------------------------------------------
Process::Process(PID id, void (*entry)(), mem::PML4T &pml4t)
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
        this->time = 0;

        this->frame = &this->frame_storage;
        memset(this->frame, 0, sizeof(*this->frame));
        this->frame->cs = 0x1b;
        this->frame->ss = 0x23;
        this->frame->flags = 1 << 9;
        this->frame->rip = reinterpret_cast<u64>(entry);
        this->frame->rsp = cpu::USER_STACK_TOP;

        this->init_kernel_stack();
}
// --------------------------------------------------

Process::Process(PID id, const Process &parent, mem::PML4T &pml4t)
{
        uptr hhdm_offset = get_kernel_hhdm_offset();

        this->id = id;
        this->pml4t = pml4t;
        this->cr3 = reinterpret_cast<u64>(pml4t.raw()) - hhdm_offset;
        this->time = 0;

        this->frame = &this->frame_storage;
        memcpy(this->frame, parent.frame, sizeof(*this->frame));
        this->frame->rax = 0;

        this->init_kernel_stack();

        this->status = ProcessStatus::Alive;
}

// --------------------------------------------------
void Process::load_pml4t(this Process &self)
{
        self.pml4t.load();
}
// --------------------------------------------------

void Process::switch_pml4t(this Process &self, const mem::PML4T &pml4t)
{
        uptr hhdm_offset = get_kernel_hhdm_offset();

        self.pml4t.destroy();
        self.pml4t = pml4t;
        self.cr3 = reinterpret_cast<u64>(self.pml4t.raw()) - hhdm_offset;
}

void Process::destroy_pml4t(this Process &self)
{
        self.pml4t.destroy();
}

void Process::save_context(this Process &self, cpu::SyscallFrame *frame)
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
        self.cr3          = frame->cr3;
}

void Process::load_context(this Process &self, cpu::SyscallFrame *frame)
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

/// Note that the stack wont be mapped if it is already mapped.
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

int Process::remove_child(this Process &self, PID id)
{
        for (usize i = 0; i < self.children.size(); i++) {
                if (self.children[i]->id == id) {
                        self.children.erase(i);
                        return 0;
                }
        }

        return -1; // no child found with this ID
}

void Process::reset_time(this Process &self)
{
        self.time = 0;
}

void Process::consume_time(this Process &self, int ms)
{
        self.time += ms;
}

u64 Process::get_time(this const Process &self)
{
        return self.time;
}

const Vector<Process *> &Process::get_children(this const Process &self)
{
        return self.children;
}

ProcessStatus Process::get_status(this const Process &self)
{
        return self.status;
}

PID Process::get_id(this const Process &self)
{
        return self.id;
}

const void *Process::get_entry(this const Process &self)
{
        return reinterpret_cast<void *>(self.entry);
}

const mem::PML4T &Process::get_pml4t(this const Process &self)
{
        return self.pml4t;
}

const ProcessStackFrame *Process::get_stack_frame(this const Process &self)
{
        return self.frame;
}

uptr Process::kernel_stack_top(this const Process &self)
{
        return self.kstack_top;
}

uptr Process::kernel_stack_frame(this const Process &self)
{
        return self.kstack_frame;
}

uptr Process::kernel_stack_pointer(this const Process &self)
{
        return self.krsp;
}

const uptr *Process::kernel_stack_pointer_address(this const Process &self)
{
        return &self.krsp;
}

bool Process::is_dead(this const Process &self)
{
        return self.status == ProcessStatus::Dead;
}

void Process::die(this Process &self)
{
        self.status = ProcessStatus::Dead;
}

} /* namespace kernel::proc */