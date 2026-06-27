#include <cpu/syscall.hpp>
#include <lib/print.hpp>
#include <lib/typing.hpp>

using kernel::lib::print_string;
using kernel::lib::u64;

namespace kernel::cpu {

namespace {

/// All types of syscalls.
enum SyscallType : u64 {
        SC_PRINT_STRING
};

} /* anonymous namespace */

/// NOTE: using a table instead of a switch loop may be more efficient... 
extern "C" void syscall_handler(SyscallFrame *frame)
{
        switch (frame->rax) {
        case SC_PRINT_STRING: {
                const char *s = reinterpret_cast<const char *>(frame->rbx);
                print_string(s);
                break;
        }

        default:
                break;
        }
}

} /* namespace kernel::cpu */