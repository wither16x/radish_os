#pragma once

#include <lib/typing.hpp>

namespace kernel::proc {

constexpr lib::usize ProcessStackSize = 0x1000; // 4 KiB

struct Process {
        void *rsp;
        lib::u8 stack[ProcessStackSize];
};

// defined in `switch.asm`
extern "C" void proc_switch(void **old_rsp, void *new_rsp);

void proc_exit();

// Note that the entry point of a process must return nothing and takes no
// parameter (this may change in the future)
void proc_init(Process *p, void (*entry)());

} /* namespace kernel::proc */