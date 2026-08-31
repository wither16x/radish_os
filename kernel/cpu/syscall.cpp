#include <cpu/syscall.hpp>
#include <lib/filesystem.hpp>
#include <lib/typing.hpp>
#include <proc/exec.hpp>
#include <proc/fork.hpp>
#include <proc/scheduler.hpp>
#include <proc/kill.hpp>
#include <proc/process.hpp>
#include <proc/wait.hpp>

#include <lib/logging.hpp>

namespace Kiwi::Cpu
{
        namespace
        {
                /// All types of syscalls.
                enum SyscallType : Lib::u64
                {
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
                        SC_GETCPUTIME,
                        SC_RM,
                        SC_SEEK,

                        SC_LIMIT // number of syscalls, always at the end of the enumeration
                };

                /// RBX = file descriptor
                /// RCX = buffer
                /// RDX = bytes to write
                void syscallWrite(SyscallFrame &frame)
                {
                        Lib::usize fd = frame.rbx;
                        Proc::Process *curr_proc = Proc::Scheduler::getCurrentProcess();

                        if (not curr_proc) {
                                frame.rax = static_cast<Lib::u64>(-1);
                                return;
                        }

                        const Lib::File *file = curr_proc->findFile(fd);
                        const void *buf = reinterpret_cast<const void *>(frame.rcx);
                        Lib::usize n = frame.rdx;

                        Fs::Vfs::Status res = Lib::write(const_cast<Lib::File *>(file), buf, n);
                        frame.rax = static_cast<Lib::u64>(res);
                }

                /// RBX = file descriptor
                /// RCX = buffer
                /// RDX = bytes to write
                void syscallRead(SyscallFrame &frame)
                {
                        Lib::usize fd = frame.rbx;

                        Proc::Process *curr_proc = Proc::Scheduler::getCurrentProcess();
                        if (not curr_proc) {
                                frame.rax = static_cast<Lib::u64>(-1);
                                return;
                        }

                        const Lib::File *file = curr_proc->findFile(fd);
                        void *buf = reinterpret_cast<void *>(frame.rcx);
                        Lib::usize n = frame.rdx;

                        Fs::Vfs::Status res = Lib::read(const_cast<Lib::File* >(file), buf, n);
                        frame.rax = static_cast<Lib::u64>(res);
                }

                /// RBX = path
                /// RCX = argc
                /// RDX = argv
                /// RDI = envp
                void syscallExec(SyscallFrame &frame)
                {
                        const char *path = reinterpret_cast<const char *>(frame.rbx);
                        int argc = frame.rcx;
                        char **argv = reinterpret_cast<char **>(frame.rdx);
                        char **envp = reinterpret_cast<char **>(frame.rdi);
                        int res = Proc::exec(path, argc, argv, envp);
                        frame.rax = res;
                        Proc::Process *current_proc = Proc::Scheduler::getCurrentProcess();
                        current_proc->loadContext(frame);
                }

                void syscallFork(SyscallFrame &frame)
                {
                        int pid = Proc::fork();
                        frame.rax = pid;
                }

                void syscallExit(SyscallFrame &frame)
                {
                        Proc::Process *proc = Proc::Scheduler::getCurrentProcess();
                        proc->die();
                        frame.rax = 0;
                        Proc::Scheduler::yield();
                }

                void syscallGetpid(SyscallFrame &frame)
                {
                        int pid = Proc::Scheduler::getCurrentProcess()->getId();
                        frame.rax = pid;
                }

                void syscallWait(SyscallFrame &frame)
                {
                        int res = Proc::wait();
                        frame.rax = res;
                }

                /// RBX = path
                void syscallOpen(SyscallFrame &frame)
                {
                        const char *path = reinterpret_cast<const char *>(frame.rbx);
                        Lib::File *f = Lib::open(path);
                        if (not f) {
                                frame.rax = static_cast<Lib::u64>(-1);
                                return;
                        }

                        Proc::Process *curr_proc = Proc::Scheduler::getCurrentProcess();

                        Lib::usize fd = curr_proc->findFd(f);
                        frame.rax = fd;
                }

                /// RBX = pointer to file descriptor
                void syscallClose(SyscallFrame &frame)
                {
                        Lib::usize fd = frame.rbx;

                        Proc::Process *curr_proc = Proc::Scheduler::getCurrentProcess();
                        if (not curr_proc) {
                                frame.rax = static_cast<Lib::u64>(-1);
                                return;
                        }

                        const Lib::File *file = curr_proc->findFile(fd);
                        Fs::Vfs::Status res = Lib::close(const_cast<Lib::File *>(file));
                        frame.rax = static_cast<Lib::u64>(res);
                }

                /// RBX = amount of pages
                ///     RBX > 0 : extend process heap
                ///     RBX = 0 : get last mapped page from process heap
                ///     RBX < 0 : shorten process heap
                void syscallLastpg(SyscallFrame &frame)
                {
                        int pages = frame.rbx;

                        Proc::Process *curr_proc = Proc::Scheduler::getCurrentProcess();
                        if (not curr_proc) {
                                frame.rax = 2; // no current process
                                return;
                        }

                        if (pages > 0) {
                                bool result = curr_proc->getHeap().extend(pages);
                                if (not result) {
                                        frame.rax = 0;
                                        return;
                                }

                                frame.rax = curr_proc->getHeap().getLastPage() - Mem::PAGE_SIZE;
                        } else if (pages == 0) {
                                frame.rax = curr_proc->getHeap().getLastPage() - Mem::PAGE_SIZE;;
                        } else if (pages < 0) {
                                bool result = curr_proc->getHeap().shorten(-pages);
                                if (not result) {
                                        frame.rax = 0;
                                        return;
                                }

                                frame.rax = curr_proc->getHeap().getLastPage() - Mem::PAGE_SIZE;
                        }
                }

                void syscallGetcputime(SyscallFrame &frame)
                {
                        Proc::Process *curr_proc = Proc::Scheduler::getCurrentProcess();
                        if (not curr_proc) {
                                frame.rax = -1;
                                return;
                        }

                        frame.rax = Proc::Scheduler::TIME_PER_PROCESS - curr_proc->getTime();
                }

                /// RBX = fd
                void syscallRm(SyscallFrame &frame)
                {
                        const char *f = reinterpret_cast<const char *>(frame.rbx);

                        Proc::Process *curr_proc = Proc::Scheduler::getCurrentProcess();
                        if (not curr_proc) {
                                frame.rax = static_cast<Lib::u64>(-1);
                                return;
                        }

                        Fs::Vfs::Status res = Lib::rm(f);
                        frame.rax = static_cast<Lib::u64>(res);
                }

                /// RBX = fd
                /// RCX = position
                /// RDX = whence (0, 1, 2)
                void syscallSeek(SyscallFrame &frame)
                {
                        Proc::Process *curr_proc = Proc::Scheduler::getCurrentProcess();
                        if (not curr_proc) {
                                frame.rax = static_cast<Lib::usize>(-1);
                                return;
                        }

                        Lib::File *file = const_cast<Lib::File *>(curr_proc->findFile(frame.rbx));
                        Lib::usize position = frame.rcx;
                        Fs::Vfs::SeekOrigin whence = static_cast<Fs::Vfs::SeekOrigin>(frame.rdx);

                        frame.rax = static_cast<Lib::usize>(file->seek(position, whence));
                }

                void (*syscalls[])(SyscallFrame &) = {
                        syscallWrite,
                        syscallRead,
                        syscallExec,
                        syscallFork,
                        syscallExit,
                        syscallGetpid,
                        syscallWait,
                        syscallOpen,
                        syscallClose,
                        syscallLastpg,
                        syscallGetcputime,
                        syscallRm,
                        syscallSeek
                };
        } // anonymous namespace

        extern "C" void syscallHandler(SyscallFrame &frame)
        {
                Proc::Process *curr_proc = Proc::Scheduler::getCurrentProcess();
                if (curr_proc)
                        curr_proc->saveContext(frame);

                if (frame.rax >= SC_LIMIT)
                        return;

                void (*handler)(SyscallFrame &) = syscalls[frame.rax];
                handler(frame);
        }
} // namespace Kiwi::Cpu