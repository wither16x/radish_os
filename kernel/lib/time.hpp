#pragma once

#include <lib/typing.hpp>

namespace kernel::lib {

void sleep(u64 ms);
// return current time in miliseconds since PIT initialization
u64 get_current_time();

} /* namespace kernel::lib */