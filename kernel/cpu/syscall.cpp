#include "fs/vfs.hpp"
#include <cpu/syscall.hpp>
#include <lib/filesystem.hpp>
#include <lib/typing.hpp>
#include <proc/exec.hpp>
#include <proc/fork.hpp>
#include <proc/scheduler.hpp>
#include <proc/kill.hpp>
#include <proc/process.hpp>
#include <proc/wait.hpp>

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
        SC_OPEN,
        SC_CLOSE,

        SC_LIMIT // number of syscalls, always at the end of the enumeration
};

/// RBX = file descriptor
/// RCX = buffer
/// RDX = bytes to write
void syscall_write(SyscallFrame *frame)
{
        usize fd = frame->rbx;
        proc::Process *curr_proc = proc::scheduler::get_current_process();
        if (!curr_proc) {
                frame->rax = static_cast<u64>(-1);
                return;
        }
        const lib::File *file = curr_proc->find_file(fd);
        const void *buf = reinterpret_cast<const void *>(frame->rcx);
        usize n = frame->rdx;
        fs::vfs::Status res = lib::write(const_cast<lib::File *>(file), buf, n);
        frame->rax = static_cast<u64>(res);
}

/// RBX = file descriptor
/// RCX = buffer
/// RDX = bytes to write
void syscall_read(SyscallFrame *frame)
{
        usize fd = frame->rbx;
        proc::Process *curr_proc = proc::scheduler::get_current_process();
        if (!curr_proc) {
                frame->rax = static_cast<u64>(-1);
                return;
        }
        const lib::File *file = curr_proc->find_file(fd);
        void *buf = reinterpret_cast<void *>(frame->rcx);
        usize n = frame->rdx;
        fs::vfs::Status res = lib::read(const_cast<lib::File* >(file), buf, n);
        frame->rax = static_cast<u64>(res);
}

/// RBX = path
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

/// RBX = path
void syscall_open(SyscallFrame *frame)
{
        const char *path = reinterpret_cast<const char *>(frame->rbx);
        lib::File *f = lib::open(path);
        if (!f) {
                frame->rax = static_cast<u64>(-1);
                return;
        }
        proc::Process *curr_proc = proc::scheduler::get_current_process();
        usize fd = curr_proc->find_fd(f);
        frame->rax = fd;
}

/// RBX = pointer to file descriptor
void syscall_close(SyscallFrame *frame)
{
        usize fd = frame->rbx;
        proc::Process *curr_proc = proc::scheduler::get_current_process();
        if (!curr_proc) {
                frame->rax = static_cast<u64>(-1);
                return;
        }
        const lib::File *file = curr_proc->find_file(fd);
        fs::vfs::Status res = lib::close(const_cast<lib::File *>(file));
        frame->rax = static_cast<u64>(res);
}

void (*syscalls[])(SyscallFrame *) = {
        syscall_write,
        syscall_read,
        syscall_exec,
        syscall_fork,
        syscall_exit,
        syscall_getpid,
        syscall_wait,
        syscall_open,
        syscall_close
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