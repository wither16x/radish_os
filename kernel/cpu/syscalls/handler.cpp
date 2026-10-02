#include <cpu/syscalls/types.hpp>
#include <cpu/syscalls/handler.hpp>
#include <cpu/syscalls/syscalls.hpp>
#include <lib/typing.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>

namespace Kiwi::Cpu::Syscalls
{
	namespace
	{
		Lib::callable<void, SyscallFrame &>
		syscalls[Lib::toUnderlying(SyscallType::SyscallCount)] = {
                        sysWrite,
                        sysRead,
                        sysExec,
                        sysFork,
                        sysExit,
                        sysGetpid,
                        sysWait,
                        sysOpen,
                        sysClose,
                        sysLastpg,
                        sysGetcputime,
                        sysRm,
                        sysSeek
                };
	} // anonymous namespace

        extern "C" void syscallHandler(SyscallFrame &frame)
        {
                Proc::Process *curr_proc = Proc::Scheduler::getCurrentProcess();
                if (curr_proc)
                        curr_proc->saveContext(frame);

                if (frame.rax >= Lib::toUnderlying(SyscallType::SyscallCount))
                        return;

                void (*handler)(SyscallFrame &) = syscalls[frame.rax];
                handler(frame);
        }
} // namespace Kiwi::Cpu::Syscalls