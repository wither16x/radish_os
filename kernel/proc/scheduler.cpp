#include <lib/time.hpp>
#include <lib/typing.hpp>
#include <proc/scheduler.hpp>
#include <proc/process.hpp>

#include <lib/logging.hpp>
using kernel::lib::log::logger;

using kernel::lib::u64, kernel::lib::usize;
using kernel::lib::get_current_time;

namespace kernel::proc::scheduler {

namespace {

Process *curr_proc = nullptr;
int curr_pid = 0;
int curr_idx = 0;

lib::Vector<Process *> processes;

void remove_current_process()
{
        delete curr_proc;
        processes.erase(curr_idx);
        curr_proc = nullptr;
}

} /* anonymous namespace */

void create_process(void (*entry)())
{
        Process *p = new Process;
        proc_init(p, curr_pid++, entry, remove_current_process);
        processes.push_back(p);
}

int execute_process(usize i)
{
        logger.debug("--> executing process %u...", i);

        if (processes.size() == 0)
                return -1;      // no processes

        logger.debug("checking bounds...");
        if (i > processes.size() - 1) {
                logger.debug("%u < %u", processes.size(), i);
                return -2; // index too big
        }

        logger.debug("checking if process is null...");
        curr_proc = processes[i];
        curr_idx = i;
        if (!curr_proc) {
                logger.debug("current process is null");
                return -3; // current process is null
        }

        logger.debug("checking process state...");
        if (curr_proc->state == ProcessState::Running)
                return -4; // current process is not idling

        u64 curr_time = get_current_time();
        if (curr_proc->timer.get_time() == 0 || !curr_proc->timer.clock())
                curr_proc->timer.reset(curr_time, curr_time + TIME_PER_PROCESS);
        
        curr_proc->state = ProcessState::Running;

        logger.debug("loading registers");
        proc_load(&curr_proc->rsp);
        logger.debug("loaded registers");

        if (!curr_proc->timer.clock()) {
                proc_save(&curr_proc->rsp);
                curr_proc->state = ProcessState::Idle;
                return -5; // no more time, switch to next process
        }

        curr_proc->entry();

        return 0;
}

} /* namespace kernel::proc::scheduler */