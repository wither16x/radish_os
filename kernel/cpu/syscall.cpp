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
        SC_WAIT,

        SC_LIMIT // number of syscalls, always at the end of the enumeration
};

void syscall_write(SyscallFrame *frame)
{
        const char *path = reinterpret_cast<const char *>(frame->rbx);
        const void *buf = reinterpret_cast<const void *>(frame->rcx);
        usize n = frame->rdx;
        int res = write(path, buf, n);
        frame->rax = res;
}

void syscall_read(SyscallFrame *frame)
{
        const char *path = reinterpret_cast<const char *>(frame->rbx);
        void *buf = reinterpret_cast<void *>(frame->rcx);
        usize n = frame->rdx;
        int res = read(path, buf, n);
        frame->rax = res;
}

void syscall_exec(SyscallFrame *frame)
{
        const char *path = reinterpret_cast<const char *>(frame->rbx);
        int res = proc::exec(path);
        frame->rax = res;
        proc::Process *current_proc = proc::scheduler::get_current_process();
        current_proc->load_context(frame);
}

void syscall_fork(SyscallFrame *frame)
{
        int pid = proc::fork();
        frame->rax = pid;
}

void syscall_exit(SyscallFrame *frame)
{
        proc::Process *proc = proc::scheduler::get_current_process();
        proc->die();
        frame->rax = 0;
        proc::scheduler::yield();
}

void syscall_getpid(SyscallFrame *frame)
{
        int pid = proc::scheduler::get_current_process()->get_id();
        frame->rax = pid;
}

void syscall_wait(SyscallFrame *frame)
{
        int res = proc::wait();
        frame->rax = res;
}

void (*syscalls[])(SyscallFrame *) = {
        syscall_write,
        syscall_read,
        syscall_exec,
        syscall_fork,
        syscall_exit,
        syscall_getpid,
        syscall_wait
};

} /* anonymous namespace */

/// NOTE: using a table instead of a switch loop may be more efficient... 
extern "C" void syscall_handler(SyscallFrame *frame)
{
        proc::Process *curr_proc = proc::scheduler::get_current_process();
        if (curr_proc)
                curr_proc->save_context(frame);

        if (frame->rax >= SC_LIMIT)
                return;

        void (*handler)(SyscallFrame *) = syscalls[frame->rax];
        handler(frame);
}

} /* namespace kernel::cpu */