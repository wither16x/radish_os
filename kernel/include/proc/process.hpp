#pragma once

#include <cpu/irq.hpp>
#include <lib/time.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>
#include <mem/pml4t.hpp>

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
class Process {
        void init_kernel_stack(this Process &self);

public:
        /// It is recommended to use `allocate_pid()` instead
        /// of assigning a PID manually
        Process(int id, void (*entry)(), mem::PML4T &pml4t);
        Process(int id, const Process &parent, mem::PML4T &pml4t);

        /// Load the process' PML4 table.
        void load(this Process &self);
        void switch_pml4t(this Process &self, const mem::PML4T &pml4t);

        void save_context(this Process &self, cpu::IRQFrame *frame);
        void load_context(this Process &self, cpu::IRQFrame *frame);

        void remap_stack(this Process &self);
        void reset_stack(this Process &self);

        void switch_entry(this Process &self, void (*entry)());

        int add_child(this Process &self, Process *child);
        int remove_child(this Process &self, int id);

        lib::Vector<Process *> children;

        ProcessStatus status;

        void (*entry)();
        int id;
        lib::u64 time;  // elapsed time in ms

        lib::u64 cr3;

        // each process has its own page tables
        mem::PML4T pml4t;

        ProcessStackFrame *frame;
        lib::uptr kstack_frame;
        lib::uptr kstack_top;
        lib::uptr krsp;
};

/// Return a PID choosen automatically.
int allocate_pid();

extern "C" void proc_switch(lib::uptr *old_rsp, lib::uptr new_rsp);
extern "C" void proc_trampoline(lib::uptr *rsp);

} /* namespace kernel::proc */