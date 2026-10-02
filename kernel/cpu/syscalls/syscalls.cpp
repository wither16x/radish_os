#include <cpu/syscalls/syscalls.hpp>
#include <lib/typing.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>
#include <proc/exec.hpp>
#include <proc/fork.hpp>
#include <proc/wait.hpp>

namespace Kiwi::Cpu::Syscalls
{
	void sysWrite(SyscallFrame &frame)
	{
		Lib::usize fd = frame.rbx;
		Proc::Process *curr_proc = Proc::Scheduler::getCurrentProcess();

		if (not curr_proc) {
			frame.rax = static_cast<Lib::u64>(-1);
			return;
		}

		const Fs::Vfs::File *file = curr_proc->findFile(fd);
		const void *buf = reinterpret_cast<const void *>(frame.rcx);
		Lib::usize n = frame.rdx;

		Fs::Vfs::Status res = Fs::Vfs::write(const_cast<Fs::Vfs::File *>(file), buf, n);
		frame.rax = static_cast<Lib::u64>(res);
	}

	void sysRead(SyscallFrame &frame)
	{
		Lib::usize fd = frame.rbx;

		Proc::Process *curr_proc = Proc::Scheduler::getCurrentProcess();
		if (not curr_proc) {
			frame.rax = static_cast<Lib::u64>(-1);
			return;
		}

		const Fs::Vfs::File *file = curr_proc->findFile(fd);
		void *buf = reinterpret_cast<void *>(frame.rcx);
		Lib::usize n = frame.rdx;

		Fs::Vfs::Status res = Fs::Vfs::read(const_cast<Fs::Vfs::File* >(file), buf, n);
		frame.rax = static_cast<Lib::u64>(res);
	}

	/// RBX = path
	/// RCX = argc
	/// RDX = argv
	/// RDI = envp
	void sysExec(SyscallFrame &frame)
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

	void sysFork(SyscallFrame &frame)
	{
		int pid = Proc::fork();
		frame.rax = pid;
	}

	void sysExit(SyscallFrame &frame)
	{
		Proc::Process *proc = Proc::Scheduler::getCurrentProcess();
		proc->die();
		frame.rax = 0;
		Proc::Scheduler::yield();
	}

	void sysGetpid(SyscallFrame &frame)
	{
		int pid = Proc::Scheduler::getCurrentProcess()->getId();
		frame.rax = pid;
	}

	void sysWait(SyscallFrame &frame)
	{
		int res = Proc::wait();
		frame.rax = res;
	}

	/// RBX = path
	void sysOpen(SyscallFrame &frame)
	{
		const char *path = reinterpret_cast<const char *>(frame.rbx);
		Fs::Vfs::File *f = Fs::Vfs::openFile(path);
		if (not f) {
			frame.rax = static_cast<Lib::u64>(-1);
			return;
		}

		Proc::Process *curr_proc = Proc::Scheduler::getCurrentProcess();

		Lib::usize fd = curr_proc->findFd(f);
		frame.rax = fd;
	}

	/// RBX = pointer to file descriptor
	void sysClose(SyscallFrame &frame)
	{
		Lib::usize fd = frame.rbx;

		Proc::Process *curr_proc = Proc::Scheduler::getCurrentProcess();
		if (not curr_proc) {
			frame.rax = static_cast<Lib::u64>(-1);
			return;
		}

		const Fs::Vfs::File *file = curr_proc->findFile(fd);
		Fs::Vfs::Status res = Fs::Vfs::closeFile(const_cast<Fs::Vfs::File *>(file));
		frame.rax = static_cast<Lib::u64>(res);
	}

	/// RBX = amount of pages
	///     RBX > 0 : extend process heap
	///     RBX = 0 : get last mapped page from process heap
	///     RBX < 0 : shorten process heap
	void sysLastpg(SyscallFrame &frame)
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

	void sysGetcputime(SyscallFrame &frame)
	{
		Proc::Process *curr_proc = Proc::Scheduler::getCurrentProcess();
		if (not curr_proc) {
			frame.rax = -1;
			return;
		}

		frame.rax = Proc::Scheduler::TIME_PER_PROCESS - curr_proc->getTime();
	}

	/// RBX = fd
	void sysRm(SyscallFrame &frame)
	{
		const char *f = reinterpret_cast<const char *>(frame.rbx);

		Proc::Process *curr_proc = Proc::Scheduler::getCurrentProcess();
		if (not curr_proc) {
			frame.rax = static_cast<Lib::u64>(-1);
			return;
		}

		Fs::Vfs::Status res = Fs::Vfs::remove(f);
		frame.rax = static_cast<Lib::u64>(res);
	}

	/// RBX = fd
	/// RCX = position
	/// RDX = whence (0, 1, 2)
	void sysSeek(SyscallFrame &frame)
	{
		Proc::Process *curr_proc = Proc::Scheduler::getCurrentProcess();
		if (not curr_proc) {
			frame.rax = static_cast<Lib::usize>(-1);
			return;
		}

		Fs::Vfs::File *file = const_cast<Fs::Vfs::File *>(curr_proc->findFile(frame.rbx));
		Lib::usize position = frame.rcx;
		Fs::Vfs::SeekOrigin whence = static_cast<Fs::Vfs::SeekOrigin>(frame.rdx);

		frame.rax = static_cast<Lib::usize>(file->seek(position, whence));
	}
} // namespace Kiwi::Cpu::Syscalls