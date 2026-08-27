#include <proc/kill.hpp>
#include <proc/scheduler.hpp>
#include <proc/process.hpp>

namespace Kiwi::Proc
{
        int kill(pid_t pid)
        {
                Process *proc = Scheduler::getProcessById(pid);
                if (not proc)
                        return -1; // process does not exit

                if (proc->getId() == Scheduler::getCurrentProcess()->getId())
                        return -2; // process is currently using the CPU

                Scheduler::undertaker(proc);

                return 0;
        }
} // namespace Kiwi::Proc