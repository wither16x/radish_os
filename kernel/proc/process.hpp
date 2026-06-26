#pragma once

#include <lib/time.hpp>
#include <lib/typing.hpp>

namespace kernel::proc {

constexpr lib::usize PROCESS_STACK_SIZE = 0x1000;       // 4 KiB

/// Representation of a process.
class Process {
public:
        /// It is recommended to use `allocate_pid()` instead
        /// of assigning a PID manually
        Process(int id, void (*entry)(), lib::u64 *pml4t);

        /// Load the process' PML4 table.
        void load(this Process &self);

        void (*entry)();
        int id;
        lib::u64 time;  // elapsed time in ms

        // CPU context
        lib::u64 r15, r14, r13, r12;
        lib::u64 r11, r10, r9, r8;
        lib::u64 rsi, rdi, rbp;
        lib::u64 rdx, rcx, rbx, rax;
        lib::u64 rip;
        lib::u64 cs;
        lib::u64 flags;
        lib::u64 rsp;
        lib::u64 ss;
        lib::u64 cr2, cr3;

        // each process has its own page tables
        lib::uptr *pml4t;
};

/// Return a PID choosen automatically.
int allocate_pid();

} /* namespace kernel::proc */