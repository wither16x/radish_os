#pragma once

#include <cpu/syscalls/frame.hpp>

namespace Kiwi::Cpu::Syscalls
{
        extern "C" void syscallHandler(SyscallFrame &frame);
} // namespace Kiwi::Cpu::Syscalls