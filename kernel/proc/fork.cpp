#include <proc/fork.hpp>
#include <proc/scheduler.hpp>
#include <proc/process.hpp>
#include <mem/pml4t.hpp>

namespace kernel::proc {

int fork()
{
        Process *parent = scheduler::get_current_process();

        mem::PML4T child_pml4t;
        child_pml4t.init(parent->pml4t);

        Process *child = new Process(allocate_pid(), *parent, child_pml4t);

        parent->add_child(child);
        scheduler::add_process(child);

        child->frame->rax = 0;

        return child->id;
}

} /* namespace kernel::proc */