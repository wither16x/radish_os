#pragma once

#include <cpu/gdt.hpp>
#include <cpu/irq.hpp>
#include <cpu/syscall.hpp>
#include <lib/time.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>
#include <lib/filesystem.hpp>
#include <lib/stack.hpp>
#include <mem/pml4t.hpp>
#include <proc/pid.hpp>
#include <proc/elf.hpp>
#include <proc/procheap.hpp>
#include <proc/procstack.hpp>
#include <fpu.hpp>

namespace Kiwi::Proc
{
        /// Alive: the process can still use the CPU
        /// Dead: the process can not use the CPU anymore, because it has aborted or exited
        enum class ProcessStatus
        {
                Alive,
                Dead
        };

        /// Representation of a process.
        /// It is recommended to use `allocate_pid_t()` instead
        /// of assigning a pid_t manually when creating a new process.
        class Process
        {
                Lib::u64                time;  // elapsed time in ms
                Lib::Vector<Process *>  children;
                ProcessStatus           status;
                Elf::elf_entry_t        entry;
                pid_t                     id;
                Lib::u64                cr3;
                Mem::PML4T              pml4t; // each process has its own page tables
                ProcessStackFrame       *frame;
                ProcessStackFrame       frame_storage;
                ProcessHeap             heap;
                ProcessKernelStack      kernel_stack;
                Lib::Stack<Lib::u8>     user_stack;
                Lib::Vector<Lib::File *> file_descriptors;
                Fpu::FpuContext fpu_context;
                int argc;
                int envc;
                char **argv;
                char **envp;

                void initKernelStack(this Process &self);
                int initArgcArgv(this Process &self, int argc, char **argv);
                int initEnvcEnvp(this Process &self, char **envp);

        public:
                /// Create a brand new process. 
                Process(pid_t id, Elf::ElfInfo *info, Mem::PML4T &pml4t);
                /// Create a process from another. Use when forking processes.
                Process(pid_t id, const Process &parent, Mem::PML4T &pml4t);

                /// Replace the file descriptors by the file descriptos from another process.
                void setFileDescriptors(this Process &self, const Process &other);
                void resetFpuContext(this Process &self);
                void initUserStack(this Process &self);
                int initArguments(this Process &self, int argc, char **argv, char **envp);
                void loadPml4t(this Process &self);
                void switchPml4t(this Process &self, const Mem::PML4T &pml4t);
                void destroyPml4t(this Process &self);
                void saveContext(this Process &self, Cpu::SyscallFrame &frame);
                void loadContext(this Process &self, Cpu::SyscallFrame &frame);
                void resetStack(this Process &self);
                void resetHeap(this Process &self, Lib::uptr start);
                void switchEntry(this Process &self, Elf::elf_entry_t entry);
                int addChild(this Process &self, Process *child);
                int removeChild(this Process &self, pid_t id);
                void resetTime(this Process &self);
                void consumeTime(this Process &self, int ms);
                void switchWith(this Process &self, const Process *other);
                void useKernelStack(this const Process &self);
                void addFileDescriptor(this Process &self, Lib::File *file);
                void removeFileDescriptor(this Process &self, Lib::File *file);
                Lib::u64 getTime(this const Process &self);
                const Lib::Vector<Process *> &getChildren(this const Process &self);
                ProcessStatus getStatus(this const Process &self);
                pid_t getId(this const Process &self);
                const void *getEntry(this const Process &self);
                const Mem::PML4T &getPml4t(this const Process &self);
                const ProcessStackFrame *getStackFrame(this const Process &self);
                const Lib::uptr *kernelStackPointerAddress(this const Process &self);
                const Lib::Vector<Lib::File *> getFileDescriptors(this const Process &self);
                const Lib::File *findFile(this const Process &self, Lib::usize id);
                Lib::usize findFd(this const Process &self, Lib::File *file);
                ProcessHeap &getHeap(this Process &self);
                ProcessKernelStack &getKernelStack(this Process &self);

                bool isDead(this const Process &self);
                void die(this Process &self);
        };

        extern "C" void __proc_switch(Lib::uptr *old_rsp, Lib::uptr new_rsp);
} // namespace Kiwi::Proc