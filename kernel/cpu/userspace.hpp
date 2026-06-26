#pragma once

#include <lib/typing.hpp>

namespace kernel::cpu {

constexpr lib::uptr USER_STACK_TOP = 0x80000000000;

extern "C" void enter_userspace(void *addr, void *rsp);

} /* namespace kernel::cpu */