#pragma once

#include <lib/typing.hpp>

namespace Kiwi::Cpu
{
        /// Registers and values pushed before calling the IRQ
        /// handler.
        struct [[gnu::packed]] IrqFrame
        {
                Lib::u64 cr2;
                Lib::u64 cr3;
                Lib::u64 rax;
                Lib::u64 rbx;
                Lib::u64 rcx;
                Lib::u64 rdx;
                Lib::u64 rsi;
                Lib::u64 rdi;
                Lib::u64 rbp;
                Lib::u64 r8;
                Lib::u64 r9;
                Lib::u64 r10;
                Lib::u64 r11;
                Lib::u64 r12;
                Lib::u64 r13;
                Lib::u64 r14;
                Lib::u64 r15;
                Lib::u64 irqno;
                Lib::u64 rip;
                Lib::u64 cs;
                Lib::u64 flags;
                Lib::u64 rsp;
                Lib::u64 ss;
        };

        void registerIrq(int n, Lib::callable<void, IrqFrame &> handler);
} // namespace Kiwi::Cpu