#include <cpu/syscall.hpp>
#include <lib/filesystem.hpp>
#include <lib/typing.hpp>
#include <proc/exec.hpp>
#include <proc/fork.hpp>
#include <proc/scheduler.hpp>
#include <proc/kill.hpp>
#include <proc/process.hpp>
#include <proc/wait.hpp>

using kernel::lib::write, kernel::lib::read;
using kernel::lib::u64, kernel::lib::usize;

namespace kernel::cpu {

namespace {

/// All types of syscalls.
enum SyscallType : u64 {
        SC_WRITE,
        SC_READ,
        SC_EXEC,
        SC_FORK,
        SC_EXIT,
        SC_GETPID,
        SC_WAIT
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
                frame->rip = current_proc->frame->rip;
                frame->rsp = current_proc->frame->rsp;
                frame->flags = current_proc->frame->flags;
                frame->cr3 = current_proc->cr3;
                frame->cs = current_proc->frame->cs;
                frame->ss = current_proc->frame->ss;
                break;
        }

        // fork:
        // no parameter
        case SC_FORK: {
                int pid = proc::fork();
                frame->rax = pid;
                break;
        }

        // exit:
        // no parameter
        case SC_EXIT: {
                proc::Process *proc = proc::scheduler::get_current_process();
                proc->status = proc::ProcessStatus::Dead;
                frame->rax = 0;
                proc::scheduler::yield();
                break;
        }

        // getpid:
        // no parameter
        case SC_GETPID: {
                int pid = proc::scheduler::get_current_process()->id;
                frame->rax = pid;
                break;
        }

        // wait:
        // no parameter
        case SC_WAIT: {
                int res = proc::wait();
                frame->rax = res;
                break;
        }

        default:
                break;
        }
}

} /* namespace kernel::cpu */