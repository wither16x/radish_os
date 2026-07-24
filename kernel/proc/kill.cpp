#include <proc/kill.hpp>
#include <proc/scheduler.hpp>
#include <proc/process.hpp>

namespace kernel::proc {

int kill(PID pid)
{
        Process *proc = scheduler::get_process_by_id(pid);
        if (!proc)
                return -1; // process does not exit

        if (proc->get_id() == scheduler::get_current_process()->get_id())
                return -2; // process is currently using the CPU

        scheduler::undertaker(proc);

        return 0;
}

} /* namespace kernel::proc */