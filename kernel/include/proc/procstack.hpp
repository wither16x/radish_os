#pragma once

#include <lib/typing.hpp>
#include <cpu/gdt.hpp>

namespace Kiwi::Proc
{
        struct [[gnu::packed]] ProcessStackFrame
        {
                Lib::u64 r15;
                Lib::u64 r14;
                Lib::u64 r13;
                Lib::u64 r12;
                Lib::u64 r11;
                Lib::u64 r10;
                Lib::u64 r9;
                Lib::u64 r8;
                Lib::u64 rdi;
                Lib::u64 rsi;
                Lib::u64 rbp;
                Lib::u64 rbx;
                Lib::u64 rdx;
                Lib::u64 rcx;
                Lib::u64 rax;
                Lib::u64 rip;
                Cpu::Segment cs;
                Lib::u64 flags;
                Lib::u64 rsp;
                Cpu::Segment ss;
        };

        /// We do not use Lib::Stack<T> here because it would be overkill.
        /// The kernel stack is mapped before scheduling is even initialized, but Lib::Stack<T>
        /// remaps the stack, and we do not want this.
        /// Also I think that adding a parameter in the Lib::Stack<T> main constructor to decide
        /// whether to map the stack or not is a bad idea, as it introduces a second state in the
        /// class.
        class ProcessKernelStack
        {
                Lib::uptr frame;
                Lib::uptr top;
                Lib::uptr *pointer;

        public:
                ProcessKernelStack();

                void push(this ProcessKernelStack &self, Lib::uptr value);

                Lib::uptr get(this const ProcessKernelStack &self);
                Lib::uptr getTop(this const ProcessKernelStack &self);
                Lib::uptr getFrame(this const ProcessKernelStack &self);
                Lib::uptr *address(this ProcessKernelStack &self);

                const Lib::uptr *address(this const ProcessKernelStack &self);
        };
} // namespace Kiwi::Proc