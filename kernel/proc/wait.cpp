#include <proc/wait.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Proc
{
        /// Yes this function waits for a child to die
        int wait()
        {
                Process *proc = Scheduler::getCurrentProcess();
                if (not proc)
                        return -1; // there is no current process

                if (proc->getChildren().size() == 0)
                        return -2; // process has no child

                Process *dead_child = nullptr;
                while (not dead_child) {
                        for (auto &child : proc->getChildren()) {
                                if (child->getStatus() == ProcessStatus::Dead) {
                                        dead_child = child;
                                        proc->removeChild(child->getId());
                                        return 0;
                                }
                        }

                        if (not dead_child)
                                Scheduler::yield();
                }

                return -3; // no dead child found
        }
} // namespace Kiwi::Proc