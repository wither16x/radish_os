#pragma once

#include <lib/typing.hpp>
#include <lib/vector.hpp>
#include <proc/process.hpp>

namespace kernel::proc::scheduler {

constexpr lib::u64 TIME_PER_PROCESS = 35; // in ms

void init();
void add_process(Process *p);
void schedule();
bool proc_time_elapsed();
void inc_proc_time(); // add 1 ms to `proc_tics`
bool is_active();

} /* namespace kernel::proc::scheduler */