#include <proc/kill.hpp>
#include <proc/scheduler.hpp>
#include <proc/process.hpp>

namespace kernel::proc {

int kill(int pid)
{
        Process *proc = scheduler::get_process_by_id(pid);
        if (!proc)
                return -1; // process does not exit

        if (proc->id == scheduler::get_current_process()->id)
                return -2; // process is currently using the CPU

        scheduler::undertaker(proc->id);

        return 0;
}

} /* namespace kernel::proc */