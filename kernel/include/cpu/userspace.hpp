#pragma once

#include <mem/page.hpp>
#include <lib/typing.hpp>

namespace kernel::cpu {

constexpr lib::uptr USER_STACK_TOP    = 0x7fffffffe000;
constexpr lib::uptr USER_STACK_SIZE   = 64 * mem::PAGE_SIZE;
constexpr lib::uptr USER_STACK_BOTTOM = USER_STACK_TOP - USER_STACK_SIZE;
constexpr lib::uptr USER_HEAP_LIMIT = USER_STACK_BOTTOM;

/// Switch the CPU to ring 3.
extern "C" void enter_userspace(void *addr, void *rsp);

} /* namespace kernel::cpu */