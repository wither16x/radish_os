#pragma once

#include <lib/typing.hpp>

namespace kernel::drivers::pit {

/// Initialize the PIT by automatically setting the number
/// of ticks per seconds.
void init();
/// Add a tic to the counter.
void tick();
/// Sleep `ms` miliseconds.
void sleep(lib::u64 ms);
/// Return the current value of the tic counter.
lib::u64 get_tics();
// Return the current value of the seconds counter.
lib::u64 get_seconds();

} /* namespace kernel::drivers::pit */