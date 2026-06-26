#pragma once

#include <lib/typing.hpp>

namespace kernel::cpu {

extern "C" void enter_userspace(void *addr, void *rsp);

} /* namespace kernel::cpu */