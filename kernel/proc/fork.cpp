#include <proc/fork.hpp>
#include <proc/scheduler.hpp>
#include <proc/process.hpp>
#include <proc/pid.hpp>
#include <mem/pml4t.hpp>
#include <cpu/assembly.hpp>

namespace kernel::proc {

PID fork()
{
        cpu::cli();

        Process *parent = scheduler::get_current_process();

        mem::PML4T child_pml4t;
        child_pml4t.init(parent->get_pml4t());

        Process *child = new Process(allocate_pid(), *parent, child_pml4t);

        parent->add_child(child);
        scheduler::add_process(child);

        cpu::sti();
        return child->get_id();
}

} /* namespace kernel::proc */