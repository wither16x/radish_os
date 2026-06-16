#pragma once

#include <lib/typing.hpp>

namespace kernel::proc {

// In this very minimalist design, each process is a structure
// containing a pointer to its entry point
struct Process {
        void (*entry)();
};

// Note that the entry point of a process must return nothing and takes no
// parameter (this may change in the future)
void proc_init(Process *p, void (*entry)());

} /* namespace kernel::proc */