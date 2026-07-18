#include <cpu/syscall.hpp>
#include <lib/filesystem.hpp>
#include <lib/typing.hpp>
#include <proc/exec.hpp>
#include <proc/scheduler.hpp>

using kernel::lib::write, kernel::lib::read;
using kernel::lib::u64, kernel::lib::usize;

namespace kernel::cpu {

namespace {

/// All types of syscalls.
enum SyscallType : u64 {
        SC_WRITE,
        SC_READ,
        SC_EXEC
};

} /* anonymous namespace */

/// NOTE: using a table instead of a switch loop may be more efficient... 
extern "C" void syscall_handler(SyscallFrame *frame)
{
        switch (frame->rax) {
        // write:
        // RBX = path
        // RCX = buffer
        // RDX = bytes
        case SC_WRITE: {
                const char *path = reinterpret_cast<const char *>(frame->rbx);
                const void *buf = reinterpret_cast<const void *>(frame->rcx);
                usize n = frame->rdx;
                int res = write(path, buf, n);
                frame->rax = res;
                break;
        }

        // read:
        // RBX = path
        // RCX = buffer
        // RDX = bytes
        case SC_READ: {
                const char *path = reinterpret_cast<const char *>(frame->rbx);
                void *buf = reinterpret_cast<void *>(frame->rcx);
                usize n = frame->rdx;
                int res = read(path, buf, n);
                frame->rax = res;
                break;
        }

        // exec:
        // RBX = path
        case SC_EXEC: {
                const char *path = reinterpret_cast<const char *>(frame->rbx);
                int res = proc::exec(path);
                frame->rax = res;
                proc::Process *current_proc = proc::scheduler::get_current_process();
                frame->rip = current_proc->rip;
                frame->rsp = current_proc->rsp;
                frame->flags = current_proc->flags;
                frame->cr3 = current_proc->cr3;
                break;
        }

        default:
                break;
        }
}

} /* namespace kernel::cpu */