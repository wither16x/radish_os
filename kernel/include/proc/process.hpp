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

namespace kernel::proc {

/// Alive: the process can still use the CPU
/// Dead: the process can not use the CPU anymore, because it has aborted or exited
enum class ProcessStatus : int {
        Alive,
        Dead
};

/// Representation of a process.
/// It is recommended to use `allocate_pid()` instead
/// of assigning a PID manually when creating a new process.
class Process {
        lib::u64                time;  // elapsed time in ms
        lib::Vector<Process *>  children;
        ProcessStatus           status;
        elf::elf_entry_t        entry;
        PID                     id;
        lib::u64                cr3;
        mem::PML4T              pml4t; // each process has its own page tables
        ProcessStackFrame       *frame;
        ProcessStackFrame       frame_storage;
        ProcessHeap             heap;
        ProcessKernelStack      kernel_stack;
        lib::Stack<lib::u8>     user_stack;
        lib::Vector<lib::File *> file_descriptors;

        void init_kernel_stack(this Process &self);

public:
        int argc;
        int envc;
        char **argv;
        char **envp;

        /// Create a brand new process. 
        Process(PID id, elf::ElfInfo *info, mem::PML4T &pml4t);
        /// Create a process from another. Use when forking processes.
        Process(PID id, const Process &parent, mem::PML4T &pml4t);

        /// Replace the file descriptors by the file descriptos from another process.
        void set_file_descriptors(this Process &self, const Process &other);

        void init_user_stack(this Process &self);
        void load_pml4t(this Process &self);
        void switch_pml4t(this Process &self, const mem::PML4T &pml4t);
        void destroy_pml4t(this Process &self);
        void save_context(this Process &self, cpu::SyscallFrame &frame);
        void load_context(this Process &self, cpu::SyscallFrame &frame);
        void reset_stack(this Process &self);
        void switch_entry(this Process &self, elf::elf_entry_t entry);
        int add_child(this Process &self, Process *child);
        int remove_child(this Process &self, PID id);
        void reset_time(this Process &self);
        void consume_time(this Process &self, int ms);
        void switch_with(this Process &self, const Process *other);
        void use_kernel_stack(this const Process &self);
        void add_file_descriptor(this Process &self, lib::File *file);
        void remove_file_descriptor(this Process &self, lib::File *file);

        lib::u64 get_time(this const Process &self);
        const lib::Vector<Process *> &get_children(this const Process &self);
        ProcessStatus get_status(this const Process &self);
        PID get_id(this const Process &self);
        const void *get_entry(this const Process &self);
        const mem::PML4T &get_pml4t(this const Process &self);
        const ProcessStackFrame *get_stack_frame(this const Process &self);
        const lib::uptr *kernel_stack_pointer_address(this const Process &self);
        const lib::Vector<lib::File *> get_file_descriptors(this const Process &self);
        const lib::File *find_file(this const Process &self, lib::usize id);
        lib::usize find_fd(this const Process &self, lib::File *file);
        ProcessHeap &get_heap(this Process &self);
        ProcessKernelStack &get_kernel_stack(this Process &self);

        bool is_dead(this const Process &self);
        void die(this Process &self);
};

extern "C" void __proc_switch(lib::uptr *old_rsp, lib::uptr new_rsp);

} /* namespace kernel::proc */