#pragma once

#include <lib/time.hpp>
#include <lib/typing.hpp>

namespace kernel::proc {

constexpr lib::u64 TIME_PER_PROCESS = 10;
constexpr lib::usize PROCESS_STACK_SIZE = 0x1000;       // 4 KiB

enum class ProcessState : int {
        Idle,
        Running
};

struct Process {
        void (*entry)();
        void (*exit)();

        int id;

        ProcessState state;
        lib::Timer timer;

        lib::u8 *stack;
        void *rsp;
};

// Note that the entry point of a process must return nothing and takes no
// parameter (this may change in the future)
void proc_init(Process *p, int id, void (*entry)(), void (*exit)());

// both functions below are defined in `switch.asm`
extern "C" void proc_save(void *rsp);
extern "C" void proc_load(void *rsp);

} /* namespace kernel::proc */