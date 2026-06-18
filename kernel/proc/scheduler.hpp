#pragma once

#include <lib/typing.hpp>
#include <lib/vector.hpp>
#include <proc/process.hpp>

namespace kernel::proc::scheduler {

constexpr lib::u64 TIME_PER_PROCESS = 35; // in ms

void init();
void add_process(Process *p);
void schedule();
// imcrease process' elapsed time
// if it reached its maximum amount of time, then reset
// the counter
// return true if the process' time is elapsed
bool inc_proc_time();
bool is_active();

} /* namespace kernel::proc::scheduler */