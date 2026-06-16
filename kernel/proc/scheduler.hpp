#pragma once

#include <lib/typing.hpp>
#include <lib/vector.hpp>
#include <proc/process.hpp>

namespace kernel::proc::scheduler {

void create_process(void (*entry)());
int execute_process(lib::usize i);

} /* namespace kernel::proc::scheduler */