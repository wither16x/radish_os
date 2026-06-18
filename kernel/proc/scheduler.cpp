#include <lib/time.hpp>
#include <lib/typing.hpp>
#include <proc/scheduler.hpp>
#include <proc/process.hpp>

#include <lib/logging.hpp>
using kernel::lib::log::logger;

using kernel::lib::u64, kernel::lib::usize;

namespace kernel::proc::scheduler {

namespace {

lib::Vector<Process *> processes;
Process *curr_proc = nullptr;
int proc_idx = -1;
bool active = false;

} /* anonymous namespace */

void init()
{
        curr_proc = processes[0];
        active = true;
}

void add_process(Process *p)
{
        // this function only adds the process to the vector:
        // it must have been initialized before
        processes.push_back(p);
        proc_idx++;
}

void schedule()
{
        // switch to the next process
        proc_idx++;
        curr_proc = processes[proc_idx];
        proc_switch(curr_proc);
}

bool inc_proc_time()
{
        if (curr_proc->time == TIME_PER_PROCESS) {
                curr_proc->time = 0;
                return true;
        }

        curr_proc->time++;
        return false;
}

bool is_active()
{
        return active;
}

} /* namespace kernel::proc::scheduler */