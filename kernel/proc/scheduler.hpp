#pragma once

#include <cpu/irq_handler.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>
#include <proc/process.hpp>

namespace kernel::proc::scheduler {

constexpr lib::u64 TIME_PER_PROCESS = 35; // in ms

void init();
void add_process(Process *p);
void tick(cpu::IRQFrame *frame);
bool is_active();
Process *get_current_process();
void set_current_process(Process *p);

} /* namespace kernel::proc::scheduler */