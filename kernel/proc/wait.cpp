#include <proc/wait.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>
#include <lib/typing.hpp>

using kernel::lib::usize;

namespace kernel::proc {

/// Yes this function waits for a child to die
int wait()
{
        Process *proc = scheduler::get_current_process();
        if (!proc)
                return -1; // there is no current process

        if (proc->get_children().size() == 0)
                return -2; // process has no child

        Process *dead_child = nullptr;
        while (!dead_child) {
                for (auto &child : proc->get_children()) {
                        if (child->status == ProcessStatus::Dead) {
                                dead_child = child;
                                proc->remove_child(child->id);
                                return 0;
                        }
                }

                if (!dead_child)
                        scheduler::yield();
        }

        return -3; // no dead child found
}

} /* namespace kernel::proc */