#include <cpu/syscall.hpp>
#include <lib/filesystem.hpp>
#include <lib/typing.hpp>

using kernel::lib::write, kernel::lib::read;
using kernel::lib::u64, kernel::lib::usize;

namespace kernel::cpu {

namespace {

/// All types of syscalls.
enum SyscallType : u64 {
        SC_WRITE,
        SC_READ
};

} /* anonymous namespace */

/// NOTE: using a table instead of a switch loop may be more efficient... 
extern "C" void syscall_handler(SyscallFrame *frame)
{
        switch (frame->rax) {
        case SC_WRITE: {
                const char *path = reinterpret_cast<const char *>(frame->rbx);
                const char *s = reinterpret_cast<const char *>(frame->rcx);
                usize n = frame->rdx;
                int res = write(path, s, n);
                frame->rax = res;
                break;
        }

        case SC_READ: {
                const char *path = reinterpret_cast<const char *>(frame->rbx);
                char *buf = reinterpret_cast<char *>(frame->rcx);
                usize n = frame->rdx;
                int res = read(path, buf, n);
                frame->rax = res;
                break;
        }

        default:
                break;
        }
}

} /* namespace kernel::cpu */