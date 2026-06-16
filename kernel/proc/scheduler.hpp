#pragma once

#include <lib/typing.hpp>
#include <lib/vector.hpp>
#include <proc/process.hpp>

namespace kernel::proc {

class Scheduler {
private:
        lib::Vector<Process *> processes;

        void execute_process(this Scheduler &self, lib::usize i);

public:
        void create_process(this Scheduler &self, void (*entry)());
        void execute(this Scheduler &self);
};

}