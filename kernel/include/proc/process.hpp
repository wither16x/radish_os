#pragma once

#include <cpu/irq.hpp>
#include <cpu/syscall.hpp>
#include <lib/time.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>
#include <mem/pml4t.hpp>
#include <proc/pid.hpp>

namespace kernel::proc {

/// Alive: the process can still use the CPU
/// Dead: the process can not use the CPU anymore, because it has aborted or exited
enum class ProcessStatus : int {
        Alive,
        Dead
};

struct [[gnu::packed]] ProcessStackFrame {
        lib::u64 r15;
        lib::u64 r14;
        lib::u64 r13;
        lib::u64 r12;
        lib::u64 r11;
        lib::u64 r10;
        lib::u64 r9;
        lib::u64 r8;
        lib::u64 rdi;
        lib::u64 rsi;
        lib::u64 rbp;
        lib::u64 rbx;
        lib::u64 rdx;
        lib::u64 rcx;
        lib::u64 rax;
        lib::u64 rip;
        lib::u64 cs;
        lib::u64 flags;
        lib::u64 rsp;
        lib::u64 ss;
};

/// Representation of a process.
/// It is recommended to use `allocate_pid()` instead
/// of assigning a PID manually when creating a new process.
class Process {
        void init_kernel_stack(this Process &self);

public:
        /// Create a brand new process. 
        Process(PID id, void (*entry)(), mem::PML4T &pml4t);
        /// Create a process from another.
        Process(PID id, const Process &parent, mem::PML4T &pml4t);

        void load_pml4t(this Process &self);
        void switch_pml4t(this Process &self, const mem::PML4T &pml4t);
        void save_context(this Process &self, cpu::SyscallFrame *frame);
        void load_context(this Process &self, cpu::SyscallFrame *frame);
        void remap_stack(this Process &self);
        void reset_stack(this Process &self);
        void switch_entry(this Process &self, void (*entry)());
        int add_child(this Process &self, Process *child);
        int remove_child(this Process &self, PID id);

        lib::Vector<Process *> children;

        ProcessStatus status;

        void (*entry)();
        PID id;
        lib::u64 time;  // elapsed time in ms

        lib::u64 cr3;

        // each process has its own page tables
        mem::PML4T pml4t;

        ProcessStackFrame *frame;
        ProcessStackFrame frame_storage;

        lib::uptr kstack_frame;
        lib::uptr kstack_top;
        lib::uptr krsp;
};

extern "C" void proc_switch(lib::uptr *old_rsp, lib::uptr new_rsp);
extern "C" void proc_trampoline();

} /* namespace kernel::proc */