#pragma once

#include <lib/typing.hpp>

namespace kernel::cpu {

/// Registers and values pushed before calling the IRQ
/// handler.
struct [[gnu::packed]] IRQFrame {
	lib::u64 cr2;
	lib::u64 cr3;
	lib::u64 rax;
	lib::u64 rbx;
	lib::u64 rcx;
	lib::u64 rdx;
	lib::u64 rsi;
	lib::u64 rdi;
	lib::u64 rbp;
	lib::u64 r8;
	lib::u64 r9;
	lib::u64 r10;
	lib::u64 r11;
	lib::u64 r12;
	lib::u64 r13;
	lib::u64 r14;
	lib::u64 r15;
        lib::u64 irqno;
	lib::u64 rip;
	lib::u64 cs;
	lib::u64 flags;
	lib::u64 rsp;
	lib::u64 ss;
};

void register_irq(int n, void (*handler)(IRQFrame *));

} /* namespace kernel::cpu */