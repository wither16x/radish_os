#pragma once

#include <lib/typing.hpp>

namespace kernel::drivers::pit {

void init();
void tick();
void sleep(lib::u64 ms);        // time in miliseconds
lib::u64 get_tics();
lib::u64 get_seconds();

} /* namespace kernel::drivers::pit */