#pragma once

#include <cpu/syscalls/frame.hpp>

namespace Kiwi::Cpu::Syscalls
{
	void sysWrite(SyscallFrame &frame);
	void sysRead(SyscallFrame &frame);
	void sysExec(SyscallFrame &frame);
	void sysFork(SyscallFrame &frame);
	void sysExit(SyscallFrame &frame);
	void sysGetpid(SyscallFrame &frame);
	void sysWait(SyscallFrame &frame);
	void sysOpen(SyscallFrame &frame);
	void sysClose(SyscallFrame &frame);
	void sysLastpg(SyscallFrame &frame);
	void sysGetcputime(SyscallFrame &frame);
	void sysRm(SyscallFrame &frame);
	void sysSeek(SyscallFrame &frame);
} // namespace Kiwi::Cpu::Syscalls