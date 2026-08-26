#include <cpu/assembly.hpp>
#include <lib/typing.hpp>
#include <lib/print.hpp>
#include <panic.hpp>

namespace Kiwi::Cpu
{
        namespace
        {
                /// Registers and values pushed before calling the exception
                /// handler.
                struct [[gnu::packed]] CpuFrame
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
                        Lib::u64 err_type;
                        Lib::u64 err_code;
                        Lib::u64 rip;
                        Lib::u64 cs;
                        Lib::u64 flags;
                        Lib::u64 rsp;
                        Lib::u64 ss;
                };
        } // anonymous namespace

        /// Handle exceptions. Basically panic and display every
        /// available information.
        extern "C" void exception_handler(CpuFrame *f)
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
} // namespace Kiwi::Cpu