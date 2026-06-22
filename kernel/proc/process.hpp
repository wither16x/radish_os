#pragma once

#include <lib/time.hpp>
#include <lib/typing.hpp>

namespace kernel::proc {

constexpr lib::usize PROCESS_STACK_SIZE = 0x1000;       // 4 KiB

struct Process {
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

        // each process has its own stack
        lib::u8 stack[PROCESS_STACK_SIZE];
        lib::uptr *pml4t;
};

// Note that the entry point of a process must return nothing and takes no
// parameter (this may change in the future)
void proc_init(Process *p, int id, void (*entry)());
// Return a PID choosen automatically
int allocate_pid();
void create_address_space(Process *p);
void load_address_space(Process *p);

} /* namespace kernel::proc */