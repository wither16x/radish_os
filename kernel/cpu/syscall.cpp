#include "mem/page.hpp"
#include <cpu/syscall.hpp>
#include <lib/filesystem.hpp>
#include <lib/typing.hpp>
#include <proc/exec.hpp>
#include <proc/fork.hpp>
#include <proc/scheduler.hpp>
#include <proc/kill.hpp>
#include <proc/process.hpp>
#include <proc/wait.hpp>

using kernel::lib::u64, kernel::lib::usize, kernel::lib::uptr;

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
        SC_LASTPG,

        SC_LIMIT // number of syscalls, always at the end of the enumeration
};

/// RBX = file descriptor
/// RCX = buffer
/// RDX = bytes to write
void syscall_write(SyscallFrame *frame)
{
        usize fd = frame->rbx;
        proc::Process *curr_proc = proc::scheduler::get_current_process();

        if (not curr_proc) {
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
        if (not curr_proc) {
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
        if (not f) {
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
        if (not curr_proc) {
                frame->rax = static_cast<u64>(-1);
                return;
        }

        const lib::File *file = curr_proc->find_file(fd);
        fs::vfs::Status res = lib::close(const_cast<lib::File *>(file));
        frame->rax = static_cast<u64>(res);
}

/// RBX = amount of pages
///     RBX > 0 : extend process heap
///     RBX = 0 : get last mapped page from process heap
///     RBX < 0 : shorten process heap
void syscall_lastpg(SyscallFrame *frame)
{
        logger.debug("syscall lastpg...");
        int pages = frame->rbx;
        logger.debug("%u page(s) requested", pages);

        proc::Process *curr_proc = proc::scheduler::get_current_process();
        if (not curr_proc) {
                frame->rax = 2; // no current process
                return;
        }

        logger.debug("process heap start = 0x%x", curr_proc->get_heap().get_start());
        logger.debug("process heap limit = 0x%x", curr_proc->get_heap().get_limit());
        logger.debug("process heap last page base = 0x%x", curr_proc->get_heap().get_last_page() - mem::PAGE_SIZE);

        if (pages > 0) {
                logger.debug("pages > 0, extending process heap...");
                bool result = curr_proc->get_heap().extend(pages);
                logger.debug("extended process heap");
                if (not result) {
                        logger.debug("failed to extend process heap");
                        frame->rax = 0;
                        return;
                }

                frame->rax = curr_proc->get_heap().get_last_page() - mem::PAGE_SIZE;
                logger.debug("extended: last page is at 0x%x", frame->rax);
        } else if (pages == 0) {
                frame->rax = curr_proc->get_heap().get_last_page() - mem::PAGE_SIZE;;
        } else if (pages < 0) {
                bool result = curr_proc->get_heap().shorten(-pages);
                if (not result) {
                        frame->rax = 0;
                        return;
                }

                logger.debug("shortened: last page is at 0x%x", frame->rax);
                frame->rax = curr_proc->get_heap().get_last_page() - mem::PAGE_SIZE;
        }
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
        syscall_close,
        syscall_lastpg
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