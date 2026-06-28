#pragma once

#include <cpu/irq.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>
#include <proc/process.hpp>

namespace kernel::proc::scheduler {

constexpr lib::u64 TIME_PER_PROCESS = 35; // in ms

/// Initialize the scheduler.
void init();
/// Add a process to the scheduler's process vector.
void add_process(Process *p);
/// Try to switch processes.
void tick(cpu::IRQFrame *frame);
/// Check if the scheduler is active/has been initialized.
bool is_active();
/// Return the currently active process.
Process *get_current_process();
/// Set the currently active process.
void set_current_process(Process *p);

} /* namespace kernel::proc::scheduler */