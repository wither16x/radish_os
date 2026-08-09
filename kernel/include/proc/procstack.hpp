#pragma once

#include <lib/typing.hpp>
#include <cpu/gdt.hpp>

namespace kernel::proc {

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
        cpu::Segment cs;
        lib::u64 flags;
        lib::u64 rsp;
        cpu::Segment ss;
};

/// We do not use lib::Stack<T> here because it would be overkill.
/// The kernel stack is mapped before scheduling is even initialized, but lib::Stack<T>
/// remaps the stack, and we do not want this.
/// Also I think that adding a parameter in the lib::Stack<T> main constructor to decide
/// whether to map the stack or not is a bad idea, as it introduces a second state in the
/// class.
class ProcessKernelStack {
        lib::uptr frame;
        lib::uptr top;
        lib::uptr *pointer;

public:
        ProcessKernelStack();

        void push(this ProcessKernelStack &self, lib::uptr value);

        lib::uptr get(this const ProcessKernelStack &self);
        lib::uptr get_top(this const ProcessKernelStack &self);
        lib::uptr get_frame(this const ProcessKernelStack &self);
        lib::uptr *address(this ProcessKernelStack &self);

        const lib::uptr *address(this const ProcessKernelStack &self);
};

} // namespace kernel::proc