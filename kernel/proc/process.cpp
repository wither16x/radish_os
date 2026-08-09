#include <cpu/gdt.hpp>
#include <kernel.hpp>
#include <mem/pml4t.hpp>
#include <cpu/irq.hpp>
#include <cpu/syscall.hpp>
#include <cpu/userspace.hpp>
#include <cpu/assembly.hpp>
#include <lib/typing.hpp>
#include <lib/memory.hpp>
#include <lib/vector.hpp>
#include <lib/filesystem.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <mem/page.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>

using kernel::lib::u8, kernel::lib::u64, kernel::lib::usize, kernel::lib::uptr;
using kernel::lib::memset, kernel::lib::memcpy;
using kernel::lib::Vector;
using kernel::lib::File;

namespace kernel::proc {

namespace {

extern "C" void __proc_trampoline();

} /* anonymous namespace */

void Process::init_kernel_stack(this Process &self)
{
        self.kernel_stack.push(static_cast<uptr>(self.frame->ss));
        self.kernel_stack.push(self.frame->rsp);
        self.kernel_stack.push(self.frame->flags);
        self.kernel_stack.push(static_cast<uptr>(self.frame->cs));
        self.kernel_stack.push(self.frame->rip);
        self.kernel_stack.push(reinterpret_cast<uptr>(&__proc_trampoline));
        self.kernel_stack.push(self.frame->rax);
        self.kernel_stack.push(self.frame->rbx);
        self.kernel_stack.push(self.frame->rcx);
        self.kernel_stack.push(self.frame->rdx);
        self.kernel_stack.push(self.frame->rsi);
        self.kernel_stack.push(self.frame->rdi);
        self.kernel_stack.push(self.frame->rbp);
        self.kernel_stack.push(self.frame->r8);
        self.kernel_stack.push(self.frame->r9);
        self.kernel_stack.push(self.frame->r10);
        self.kernel_stack.push(self.frame->r11);
        self.kernel_stack.push(self.frame->r12);
        self.kernel_stack.push(self.frame->r13);
        self.kernel_stack.push(self.frame->r14);
        self.kernel_stack.push(self.frame->r15);
}

void Process::init_user_stack(this Process &self)
{
        uptr hhdm_offset = get_kernel_hhdm_offset();

        uptr *argv_uaddrs = self.argc > 0 ? new uptr[self.argc] : nullptr;
        uptr *envp_uaddrs = self.envc > 0 ? new uptr[self.envc] : nullptr;

        if (self.envp) {
                for (int i = self.envc - 1; i >= 0; i--) {
                        usize len = lib::strlen(self.envp[i]) + 1;
                        self.user_stack.grow(len);

                        for (usize j = 0; j < len; j++) {
                                uptr uaddr = reinterpret_cast<uptr>(self.user_stack.get()) + j;
                                uptr kaddr = self.user_stack.virt_to_phys(uaddr) + hhdm_offset;
                                *reinterpret_cast<char *>(kaddr) = self.envp[i][j];
                        }

                        envp_uaddrs[i] = reinterpret_cast<uptr>(self.user_stack.get());
                }
        }

        if (self.argv) {
                for (int i = self.argc - 1; i >= 0; i--) {
                        usize len = lib::strlen(self.argv[i]) + 1;
                        self.user_stack.grow(len);

                        for (usize j = 0; j < len; j++) {
                                uptr uaddr = reinterpret_cast<uptr>(self.user_stack.get()) + j;
                                uptr kaddr = self.user_stack.virt_to_phys(uaddr) + hhdm_offset;
                                *reinterpret_cast<char *>(kaddr) = self.argv[i][j];
                        }

                        argv_uaddrs[i] = reinterpret_cast<uptr>(self.user_stack.get());
                }
        }

        self.user_stack.align(16);

        int total_words = self.envc + self.argc + 5;
        if (total_words % 2 != 0) {
                self.user_stack.grow(sizeof(u64));
                uptr uaddr = reinterpret_cast<uptr>(self.user_stack.get());
                uptr kaddr = self.user_stack.virt_to_phys(uaddr) + hhdm_offset;
                *reinterpret_cast<u64 *>(kaddr) = 0;
        }

        for (int i = 0; i < 2; i++) {
                self.user_stack.grow(sizeof(u64));
                uptr uaddr = reinterpret_cast<uptr>(self.user_stack.get());
                uptr kaddr = self.user_stack.virt_to_phys(uaddr) + hhdm_offset;
                *reinterpret_cast<u64 *>(kaddr) = 0;
        }

        self.user_stack.grow(sizeof(u64));
        {
                uptr uaddr = reinterpret_cast<uptr>(self.user_stack.get());
                uptr kaddr = self.user_stack.virt_to_phys(uaddr) + hhdm_offset;
                *reinterpret_cast<u64 *>(kaddr) = 0;
        }
        for (int i = self.envc - 1; i >= 0; i--) {
                self.user_stack.grow(sizeof(u64));
                uptr uaddr = reinterpret_cast<uptr>(self.user_stack.get());
                uptr kaddr = self.user_stack.virt_to_phys(uaddr) + hhdm_offset;
                *reinterpret_cast<u64 *>(kaddr) = envp_uaddrs[i];
        }

        self.user_stack.grow(sizeof(u64));
        {
                uptr uaddr = reinterpret_cast<uptr>(self.user_stack.get());
                uptr kaddr = self.user_stack.virt_to_phys(uaddr) + hhdm_offset;
                *reinterpret_cast<u64 *>(kaddr) = 0;
        }
        for (int i = self.argc - 1; i >= 0; i--) {
                self.user_stack.grow(sizeof(u64));
                uptr uaddr = reinterpret_cast<uptr>(self.user_stack.get());
                uptr kaddr = self.user_stack.virt_to_phys(uaddr) + hhdm_offset;
                *reinterpret_cast<u64 *>(kaddr) = argv_uaddrs[i];
        }

        self.user_stack.grow(sizeof(u64));
        {
                uptr uaddr = reinterpret_cast<uptr>(self.user_stack.get());
                uptr kaddr = self.user_stack.virt_to_phys(uaddr) + hhdm_offset;
                *reinterpret_cast<u64 *>(kaddr) = static_cast<u64>(self.argc);
        }

        delete[] argv_uaddrs;
        delete[] envp_uaddrs;

        self.frame->rsp = reinterpret_cast<u64>(self.user_stack.get());
}

// --------------------------------------------------
Process::Process(PID id, elf::ElfInfo *info, mem::PML4T &pml4t)
        : pml4t(pml4t),
        heap(mem::page_align_up(info->highest_vaddr), mem::page_align_up(info->highest_vaddr), cpu::USER_HEAP_LIMIT, this->pml4t),
        user_stack(this->pml4t, cpu::USER_STACK_BOTTOM, cpu::USER_STACK_TOP)
{
        this->id                = id;
        this->entry             = info->entry;
        this->status            = ProcessStatus::Alive;
        this->time              = 0;
        this->argc              = 0;
        this->envc              = 0;
        this->argv              = nullptr;
        this->envp              = nullptr;
        this->frame             = &this->frame_storage;
        memset(this->frame, 0, sizeof(*this->frame));
        this->frame->cs         = cpu::Segment::UserCS;
        this->frame->ss         = cpu::Segment::UserSS;
        this->frame->flags      = 1 << 9;
        this->frame->rip        = reinterpret_cast<u64>(entry);
        this->frame->rsp        = cpu::USER_STACK_TOP;

        this->init_user_stack();
        this->init_kernel_stack();
}
// --------------------------------------------------

Process::Process(PID id, const Process &parent, mem::PML4T &pml4t)
        : pml4t(pml4t),
        heap(parent.heap.get_start(), parent.heap.get_last_page(), parent.heap.get_limit(), this->pml4t),
        user_stack(parent.user_stack, this->pml4t)
{
        uptr hhdm_offset = get_kernel_hhdm_offset();
        
        this->id                = id;
        this->cr3               = reinterpret_cast<u64>(this->pml4t.raw()) - hhdm_offset;
        this->time              = 0;
        this->frame             = &this->frame_storage;
        memcpy(this->frame, parent.frame, sizeof(*this->frame));
        this->frame->rax        = 0;
        this->status            = ProcessStatus::Alive;

        this->set_file_descriptors(parent);
        this->init_kernel_stack();
}

void Process::set_file_descriptors(this Process &self, const Process &other)
{
        self.file_descriptors = other.file_descriptors;

        for (auto &fd : self.file_descriptors) {
                if (fd)
                        ++fd->ref_count;
        }
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

void Process::save_context(this Process &self, cpu::SyscallFrame &frame)
{
        self.frame->rax   = frame.rax;
        self.frame->rbx   = frame.rbx;
        self.frame->rcx   = frame.rcx;
        self.frame->rdx   = frame.rdx;
        self.frame->rsi   = frame.rsi;
        self.frame->rdi   = frame.rdi;
        self.frame->rbp   = frame.rbp;
        self.frame->r8    = frame.r8;
        self.frame->r9    = frame.r9;
        self.frame->r10   = frame.r10;
        self.frame->r11   = frame.r11;
        self.frame->r12   = frame.r12;
        self.frame->r13   = frame.r13;
        self.frame->r14   = frame.r14;
        self.frame->r15   = frame.r15;
        self.frame->rip   = frame.rip;
        self.frame->cs    = static_cast<cpu::Segment>(frame.cs);
        self.frame->flags = frame.flags | (1 << 9);
        self.frame->rsp   = frame.rsp;
        self.frame->ss    = static_cast<cpu::Segment>(frame.ss);
        self.cr3          = frame.cr3;
}

void Process::load_context(this Process &self, cpu::SyscallFrame &frame)
{
        frame.rax      = self.frame->rax;
        frame.rbx      = self.frame->rbx;
        frame.rcx      = self.frame->rcx;
        frame.rdx      = self.frame->rdx;
        frame.rsi      = self.frame->rsi;
        frame.rdi      = self.frame->rdi;
        frame.rbp      = self.frame->rbp;
        frame.r8       = self.frame->r8;
        frame.r9       = self.frame->r9;
        frame.r10      = self.frame->r10;
        frame.r11      = self.frame->r11;
        frame.r12      = self.frame->r12;
        frame.r13      = self.frame->r13;
        frame.r14      = self.frame->r14;
        frame.r15      = self.frame->r15;
        frame.rip      = self.frame->rip;
        frame.cs       = static_cast<u64>(self.frame->cs);
        frame.flags    = self.frame->flags | (1 << 9);
        frame.rsp      = self.frame->rsp;
        frame.ss       = static_cast<u64>(self.frame->ss);
        frame.cr3      = self.cr3;
}

void Process::reset_stack(this Process &self)
{
        self.user_stack.reset(self.pml4t, cpu::USER_STACK_BOTTOM, cpu::USER_STACK_TOP);
        self.frame->rsp = cpu::USER_STACK_TOP;
}

void Process::switch_entry(this Process &self, elf::elf_entry_t entry)
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

void Process::switch_with(this Process &self, const Process *other)
{
        __proc_switch(self.kernel_stack.address(), other->kernel_stack.get());
}

void Process::use_kernel_stack(this const Process &self)
{
        get_kernel_gdt().get_tss().reset_stack(self.kernel_stack.get());
}

void Process::add_file_descriptor(this Process &self, File *file)
{
        for (usize i = 0; i < self.file_descriptors.size(); i++) {
                if (!self.file_descriptors[i]) {
                        self.file_descriptors[i] = file;
                        return;
                }
        }
        self.file_descriptors.push_back(file);
}

void Process::remove_file_descriptor(this Process &self, File *file)
{
        for (usize i = 0; i < self.file_descriptors.size(); i++) {
                if (self.file_descriptors[i] == file) {
                        self.file_descriptors[i] = nullptr;
                        break;
                }
        }
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
        return self.kernel_stack.get_top();
}

uptr Process::kernel_stack_frame(this const Process &self)
{
        return self.kernel_stack.get_frame();
}

uptr Process::kernel_stack_pointer(this const Process &self)
{
        return self.kernel_stack.get();
}

const uptr *Process::kernel_stack_pointer_address(this const Process &self)
{
        return self.kernel_stack.address();
}

const Vector<File *> Process::get_file_descriptors(this const Process &self)
{
        return self.file_descriptors;
}

const File *Process::find_file(this const Process &self, usize id)
{
        if (id >= self.file_descriptors.size())
                return nullptr;
        return self.file_descriptors[id];
}

usize Process::find_fd(this const Process &self, File *file)
{
        for (usize i = 0; i < self.file_descriptors.size(); i++) {
                if (self.file_descriptors[i] == file)
                        return i;
        }

        // note: this value is going to be converted to the biggest value that usize
        // can handle
        return -1; // not found
}

ProcessHeap &Process::get_heap(this Process &self)
{
        return self.heap;
}

bool Process::is_dead(this const Process &self)
{
        return self.status == ProcessStatus::Dead;
}

void Process::die(this Process &self)
{
        for (usize i = 0; i < self.file_descriptors.size(); i++) {
                if (self.file_descriptors[i])
                        lib::close(self.file_descriptors[i]);
        }

        self.status = ProcessStatus::Dead;
}

} /* namespace kernel::proc */