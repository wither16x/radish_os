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
                        "CPU exception #{} with error code {}\r\n"
                        "RAX=0x{} RBX=0x{} RCX=0x{} RDX=0x{}\r\n"
                        "RDI=0x{} RSI=0x{}\r\n"
                        "RBP=0x{} RSP=0x{}\r\n"
                        "R8=0x{} R9=0x{} R10=0x{} R11=0x{} R12=0x{} R13=0x{} R14=0x{} R15=0x{}\r\n"
                        "CR2=0x{} CR3=0x{}\r\n"
                        "CS=0x{} SS=0x{}\r\n"
                        "RIP=0x{}\r\n"
                        "FLAGS=0x{}\r\n",
                        f->err_type, f->err_code,
                        Lib::hex(f->rax), Lib::hex(f->rbx), Lib::hex(f->rcx), Lib::hex(f->rdx),
                        Lib::hex(f->rdi), Lib::hex(f->rsi),
                        Lib::hex(f->rbp), Lib::hex(f->rsp),
                        Lib::hex(f->r8), Lib::hex(f->r9), Lib::hex(f->r10), Lib::hex(f->r11), Lib::hex(f->r12), Lib::hex(f->r13), Lib::hex(f->r14), Lib::hex(f->r15),
                        Lib::hex(f->cr2), Lib::hex(f->cr3),
                        Lib::hex(f->cs), Lib::hex(f->ss),
                        Lib::hex(f->rip),
                        Lib::hex(f->flags)
                );
        }
} // namespace Kiwi::Cpu