#pragma once

#include <lib/typing.hpp>

namespace kernel::lib {

/// Sleep `ms` miliseconds.
void sleep(u64 ms);
/// Return the current time in miliseconds since
/// timer initialization.
u64 get_current_time();

} /* namespace kernel::lib */