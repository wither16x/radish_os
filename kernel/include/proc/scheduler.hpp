#pragma once

#include <lib/typing.hpp>
#include <lib/vector.hpp>
#include <proc/process.hpp>

namespace kernel::proc::scheduler {

constexpr lib::u64 TIME_PER_PROCESS = 35; // in ms

/// Initialize the scheduler.
void init();
/// Add a process to the scheduler's process vector.
void add_process(Process *p);
/// Remove a process from the scheduler's process vector.
void remove_process(int pid);
/// Try to switch processes.
void tick();
/// Check if the scheduler is active/has been initialized.
bool is_active();
/// Return the currently active process.
Process *get_current_process();
/// Return any process or nullptr by its ID.
Process *get_process_by_id(int pid);
/// Set the currently active process.
void set_current_process(Process *p);
/// Return the scheduler's process vector.
const lib::Vector<Process *> &get_processes();
/// Clean the process with the ID `pid` if it is dead.
void undertaker(int pid);
void yield();

} /* namespace kernel::proc::scheduler */