#include <lib/typing.hpp>
#include <proc/scheduler.hpp>
#include <proc/process.hpp>

using kernel::lib::usize;

namespace kernel::proc {

// This very simple scheduler just calls every process in order
// until there is no more

void Scheduler::execute_process(this Scheduler &self, usize i)
{
        self.processes[i]->entry();
}

void Scheduler::create_process(this Scheduler &self, void (*entry)())
{
        Process *p = new Process;
        proc_init(p, entry);
        self.processes.push_back(p);
}

void Scheduler::execute(this Scheduler &self)
{
        for (usize i = 0; i < self.processes.size(); i++)
                self.execute_process(i);
}

}