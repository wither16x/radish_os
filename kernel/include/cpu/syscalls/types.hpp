#pragma once

#include <lib/typing.hpp>

namespace Kiwi::Cpu::Syscalls
{
	enum class SyscallType : Lib::u64
	{
		Write,
		Read,
		Exec,
		Fork,
		Exit,
		Getpid,
		Wait,
		Open,
		Close,
		Lastpg,
		Getcputime,
		Rm,
		Seek,

		SyscallCount // number of syscalls
	};
} // namespace Kiwi::Cpu::Syscalls