#pragma once

#include <mem/vmm.hpp>
#include <lib/typing.hpp>

namespace kernel::cpu {

constexpr lib::uptr USER_STACK_TOP    = 0x7fffffffe000;
constexpr lib::uptr USER_STACK_SIZE   = 64 * mem::vmm::PAGE_BYTES;
constexpr lib::uptr USER_STACK_BOTTOM = USER_STACK_TOP - USER_STACK_SIZE;

extern "C" void enter_userspace(void *addr, void *rsp);

} /* namespace kernel::cpu */