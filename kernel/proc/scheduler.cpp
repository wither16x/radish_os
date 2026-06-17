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
        active = true;
}

void add_process(Process *p)
{
        // this function only adds the process to the vector:
        // it must have been initialized before
        processes.push_back(p);
        proc_idx++;
        logger.debug("added process PID=%u, proc_idx=%d", p->id, proc_idx);
}

void schedule()
{
        // switch to the next process
        proc_save(curr_proc->rsp);
        proc_idx++;
        curr_proc = processes[proc_idx];
        proc_load(curr_proc->rsp);
        curr_proc->entry();
        curr_proc->exit();
}

bool proc_time_elapsed()
{
        return curr_proc->time == TIME_PER_PROCESS;
}

void inc_proc_time()
{
        curr_proc->time++;
}

bool is_active()
{
        return active;
}

} /* namespace kernel::proc::scheduler */