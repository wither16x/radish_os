#include <cpu/assembly.hpp>
#include <lib/typing.hpp>
#include <panic.hpp>

using kernel::lib::u64;

namespace kernel::cpu {

namespace {

// Registers and values pushed before calling `exception_handler()`
struct [[gnu::packed]] CPUFrame {
	u64 cr2;
	u64 cr3;
	u64 rax;
	u64 rbx;
	u64 rcx;
	u64 rdx;
	u64 rsi;
	u64 rdi;
	u64 rbp;
	u64 r8;
	u64 r9;
	u64 r10;
	u64 r11;
	u64 r12;
	u64 r13;
	u64 r14;
	u64 r15;
	u64 err_type;
	u64 err_code;
	u64 rip;
	u64 cs;
	u64 flags;
	u64 rsp;
	u64 ss;
};

} /* anonymous namespace */

extern "C" [[gnu::noreturn]] void exception_handler(CPUFrame *f)
{
        panic(
                "CPU exception #%u with error code %u\r\n"
                "RAX=0x%x RBX=0x%x RCX=0x%x RDX=0x%x\r\n"
                "RDI=0x%x RSI=0x%x\r\n"
                "RBP=0x%x RSP=0x%x\r\n"
                "R8=0x%x R9=0x%x R10=0x%x R11=0x%x R12=0x%x R13=0x%x R14=0x%x R15=0x%x\r\n"
                "CR2=0x%x CR3=0x%x\r\n"
                "CS=0x%x SS=0x%x\r\n"
                "RIP=0x%x\r\n"
                "FLAGS=0x%x\r\n",
                f->err_type, f->err_code,
                f->rax, f->rbx, f->rcx, f->rdx,
                f->rdi, f->rsi,
                f->rbp, f->rsp,
                f->r8, f->r9, f->r10, f->r11, f->r12, f->r13, f->r14, f->r15,
                f->cr2, f->cr3,
                f->cs, f->ss,
                f->rip,
                f->flags
        );
}

} /* namespace kernel::cpu */