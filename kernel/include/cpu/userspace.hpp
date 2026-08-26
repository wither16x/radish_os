#pragma once

#include <mem/page.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Cpu
{
        constexpr Lib::uptr USER_STACK_TOP    = 0x7fffffffe000;
        constexpr Lib::uptr USER_STACK_SIZE   = 64 * Mem::PAGE_SIZE;
        constexpr Lib::uptr USER_STACK_BOTTOM = USER_STACK_TOP - USER_STACK_SIZE;
        constexpr Lib::uptr USER_HEAP_LIMIT = USER_STACK_BOTTOM;

        /// Switch the CPU to ring 3.
        extern "C" void enter_userspace(void *addr, void *rsp);
} // namespace Kiwi::Cpu