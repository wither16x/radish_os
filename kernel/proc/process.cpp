#include <cpu/gdt.hpp>
#include <cpu/cpu.hpp>
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
#include <proc/procstack.hpp>
#include <proc/procheap.hpp>

namespace Kiwi::Proc
{
        namespace
        {
                extern "C" void __proc_trampoline();
        } // anonymous namespace

        void Process::initKernelStack(this Process &self)
        {
                self.kernel_stack.push(static_cast<Lib::uptr>(self.frame->ss));
                self.kernel_stack.push(self.frame->rsp);
                self.kernel_stack.push(self.frame->flags);
                self.kernel_stack.push(static_cast<Lib::uptr>(self.frame->cs));
                self.kernel_stack.push(self.frame->rip);
                self.kernel_stack.push(reinterpret_cast<Lib::uptr>(&__proc_trampoline));
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

        int Process::initArgcArgv(this Process &self, int argc, char **argv)
        {
                if (not argv)
                        return -1;

                self.argv = new char *[argc + 1];
                if (not self.argv) {
                        Cpu::enableInterrupts();
                        return -2;
                }

                for (int i = 0; i < argc; i++) {
                        self.argv[i] = new char[Lib::strlen(argv[i]) + 1];
                        Lib::strcpy(argv[i], self.argv[i]);
                }

                self.argv[argc] = nullptr;
                self.argc = argc;

                return 0;
        }

        int Process::initEnvcEnvp(this Process &self, char **envp)
        {
                if (not envp)
                        return -1;

                int envc = 0;
                while (envp[envc] != nullptr)
                        ++envc;

                self.envp = new char *[envc + 1];
                if (not self.envp) {
                        Cpu::enableInterrupts();
                        return -2;
                }

                for (int i = 0; i < envc; i++) {
                        self.envp[i] = new char[Lib::strlen(envp[i]) + 1];
                        Lib::strcpy(envp[i], self.envp[i]);
                }

                self.envp[envc] = nullptr;
                self.envc = envc;

                return 0;
        }

        Process::Process(pid_t id, Elf::ElfInfo *info, Mem::PML4T &pml4t)
                : pml4t(pml4t),
                heap(Mem::pageAlignUp(info->highest_vaddr), Mem::pageAlignUp(info->highest_vaddr), Cpu::USER_HEAP_LIMIT, this->pml4t),
                user_stack(this->pml4t, Cpu::USER_STACK_BOTTOM, Cpu::USER_STACK_TOP)
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
                Lib::memset(this->frame, 0, sizeof(*this->frame));
                this->frame->cs         = Cpu::Segment::UserCs;
                this->frame->ss         = Cpu::Segment::UserSs;
                this->frame->flags      = 1 << 9;
                this->frame->rip        = reinterpret_cast<Lib::u64>(entry);
                this->frame->rsp        = Cpu::USER_STACK_TOP;

                this->initUserStack();
                this->initKernelStack();
        }

        Process::Process(pid_t id, const Process &parent, Mem::PML4T &pml4t)
                : pml4t(pml4t),
                heap(parent.heap.getStart(), parent.heap.getLastPage(), parent.heap.getLimit(), this->pml4t),
                user_stack(parent.user_stack, this->pml4t)
        {
                Lib::uptr hhdm_offset = getKernelHhdmOffset();
                
                this->id                = id;
                this->cr3               = reinterpret_cast<Lib::u64>(this->pml4t.raw()) - hhdm_offset;
                this->time              = 0;
                this->frame             = &this->frame_storage;
                Lib::memcpy(this->frame, parent.frame, sizeof(*this->frame));
                this->frame->rax        = 0;
                this->status            = ProcessStatus::Alive;

                this->setFileDescriptors(parent);
                this->initKernelStack();
        }

        void Process::initUserStack(this Process &self)
        {
                Lib::uptr *envp_uaddrs = self.user_stack.push_string_array(self.envp, self.envc);
                Lib::uptr *argv_uaddrs = self.user_stack.push_string_array(self.argv, self.argc);

                self.user_stack.align(16);

                int total_words = self.envc + self.argc + 5;
                if (total_words % 2 != 0)
                        self.user_stack.push_qword(0);

                // TODO: full auxv
                for (int i = 0; i < 2; i++)
                        self.user_stack.push_qword(0);

                self.user_stack.push_qword(0);
                for (int i = self.envc - 1; i >= 0; i--)
                        self.user_stack.push_qword(envp_uaddrs[i]);

                self.user_stack.push_qword(0);
                for (int i = self.argc - 1; i >= 0; i--)
                        self.user_stack.push_qword(argv_uaddrs[i]);

                self.user_stack.push_qword(self.argc);

                delete[] argv_uaddrs;
                delete[] envp_uaddrs;

                self.frame->rsp = reinterpret_cast<Lib::u64>(self.user_stack.get());
        }

        void Process::setFileDescriptors(this Process &self, const Process &other)
        {
                self.file_descriptors = other.file_descriptors;

                for (auto &fd : self.file_descriptors) {
                        if (fd)
                                ++fd->ref_count;
                }
        }

        void Process::resetFpuContext(this Process &self)
        {
                Lib::memset(&self.fpu_context, 0, sizeof(self.fpu_context));
                self.fpu_context.mxcsr = 0x1f80;
                self.fpu_context.fcw = 0x37f;
        }

        int Process::initArguments(this Process &self, int argc, char **argv, char **envp)
        {
                if (self.initArgcArgv(argc, argv) != 0) {
                        self.argc = 0;
                        self.argv = nullptr;
                }

                if (self.initEnvcEnvp(envp) != 0) {
                        self.envc = 0;
                        self.envp = nullptr;
                }

                return 0;
        }

        void Process::loadPml4t(this Process &self)
        {
                self.pml4t.load();
        }

        void Process::switchPml4t(this Process &self, const Mem::PML4T &pml4t)
        {
                Lib::uptr hhdm_offset = getKernelHhdmOffset();

                self.pml4t.destroy();
                self.pml4t = pml4t;
                self.cr3 = reinterpret_cast<Lib::u64>(self.pml4t.raw()) - hhdm_offset;
        }

        void Process::destroyPml4t(this Process &self)
        {
                self.pml4t.destroy();
        }

        void Process::saveContext(this Process &self, Cpu::SyscallFrame &frame)
        {
                __asm__ volatile (
                        "fxsave %0"
                        ::
                        "m"(self.fpu_context)
                );
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
                self.frame->cs    = static_cast<Cpu::Segment>(frame.cs);
                self.frame->flags = frame.flags | (1 << 9);
                self.frame->rsp   = frame.rsp;
                self.frame->ss    = static_cast<Cpu::Segment>(frame.ss);
                self.cr3          = frame.cr3;
        }

        void Process::loadContext(this Process &self, Cpu::SyscallFrame &frame)
        {
                __asm__ volatile (
                        "fxrstor %0"
                        ::
                        "m"(self.fpu_context)
                );
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
                frame.cs       = static_cast<Lib::u64>(self.frame->cs);
                frame.flags    = self.frame->flags | (1 << 9);
                frame.rsp      = self.frame->rsp;
                frame.ss       = static_cast<Lib::u64>(self.frame->ss);
                frame.cr3      = self.cr3;
        }

        void Process::resetStack(this Process &self)
        {
                self.user_stack.reset(self.pml4t, Cpu::USER_STACK_BOTTOM, Cpu::USER_STACK_TOP);
                self.frame->rsp = Cpu::USER_STACK_TOP;
        }

        void Process::resetHeap(this Process &self, Lib::uptr start)
        {
                self.heap.reset(start, self.pml4t);
        }

        void Process::switchEntry(this Process &self, Elf::elf_entry_t entry)
        {
                self.entry = entry;
                self.frame->rip = reinterpret_cast<Lib::u64>(entry);
        }

        int Process::addChild(this Process &self, Process *child)
        {
                for (auto &proc : self.children) {
                        if (proc->id == child->id)
                                return -1; // already has a child with same ID, probably itself
                }

                self.children.pushBack(child);

                return 0;
        }

        int Process::removeChild(this Process &self, pid_t id)
        {
                for (Lib::usize i = 0; i < self.children.size(); i++) {
                        if (self.children[i]->id == id) {
                                self.children.erase(i);
                                return 0;
                        }
                }

                return -1; // no child found with this ID
        }

        void Process::resetTime(this Process &self)
        {
                self.time = 0;
        }

        void Process::consumeTime(this Process &self, int ms)
        {
                self.time += ms;
        }

        void Process::switchWith(this Process &self, const Process *other)
        {
                __proc_switch(self.kernel_stack.address(), other->kernel_stack.get());
        }

        void Process::useKernelStack(this const Process &self)
        {
                get_kernel_gdt().getTss().resetStack(self.kernel_stack.get());
        }

        void Process::addFileDescriptor(this Process &self, Lib::File *file)
        {
                for (Lib::usize i = 0; i < self.file_descriptors.size(); i++) {
                        if (not self.file_descriptors[i]) {
                                self.file_descriptors[i] = file;
                                return;
                        }
                }
                // if no free slot, extend the vector
                self.file_descriptors.pushBack(file);
        }

        void Process::removeFileDescriptor(this Process &self, Lib::File *file)
        {
                for (Lib::usize i = 0; i < self.file_descriptors.size(); i++) {
                        if (self.file_descriptors[i] == file) {
                                self.file_descriptors[i] = nullptr;
                                return;
                        }
                }
        }

        Lib::u64 Process::getTime(this const Process &self)
        {
                return self.time;
        }

        const Lib::Vector<Process *> &Process::getChildren(this const Process &self)
        {
                return self.children;
        }

        ProcessStatus Process::getStatus(this const Process &self)
        {
                return self.status;
        }

        pid_t Process::getId(this const Process &self)
        {
                return self.id;
        }

        const void *Process::getEntry(this const Process &self)
        {
                return reinterpret_cast<void *>(self.entry);
        }

        const Mem::PML4T &Process::getPml4t(this const Process &self)
        {
                return self.pml4t;
        }

        const ProcessStackFrame *Process::getStackFrame(this const Process &self)
        {
                return self.frame;
        }

        const Lib::uptr *Process::kernelStackPointerAddress(this const Process &self)
        {
                return self.kernel_stack.address();
        }

        const Lib::Vector<Lib::File *> Process::getFileDescriptors(this const Process &self)
        {
                return self.file_descriptors;
        }

        const Lib::File *Process::findFile(this const Process &self, Lib::usize id)
        {
                if (id >= self.file_descriptors.size())
                        return nullptr;
                return self.file_descriptors[id];
        }

        Lib::usize Process::findFd(this const Process &self, Lib::File *file)
        {
                for (Lib::usize i = 0; i < self.file_descriptors.size(); i++) {
                        if (self.file_descriptors[i] == file)
                                return i;
                }

                // note: this value is going to be converted to the biggest value that Lib::usize
                // can handle
                return -1; // not found
        }

        ProcessHeap &Process::getHeap(this Process &self)
        {
                return self.heap;
        }

        ProcessKernelStack &Process::getKernelStack(this Process &self)
        {
                return self.kernel_stack;
        }

        bool Process::isDead(this const Process &self)
        {
                return self.status == ProcessStatus::Dead;
        }

        void Process::die(this Process &self)
        {
                for (Lib::usize i = 0; i < self.file_descriptors.size(); i++) {
                        if (self.file_descriptors[i])
                                self.file_descriptors[i]->close();
                }

                self.status = ProcessStatus::Dead;
        }
} // namespace Kiwi::Proc