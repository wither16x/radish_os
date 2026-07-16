#include "lib/alloc.hpp"
#include <cpu/irq.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>
#include <proc/scheduler.hpp>
#include <proc/process.hpp>

using kernel::lib::u64, kernel::lib::usize;
using kernel::lib::log::logger;

namespace kernel::proc::scheduler {

namespace {

lib::Vector<Process *> processes;
Process *curr_proc = nullptr;
usize curr_proc_idx = 0;
bool active = false;

} /* anonymous namespace */

// --------------------------------------------------
void init()
{
        curr_proc_idx = 0;
        curr_proc = nullptr;
        active = true;

        logger.ok("initialized scheduler");
}
// --------------------------------------------------

// --------------------------------------------------
void add_process(Process *p)
{
        // this function only adds the process to the vector:
        // it must have been initialized before
        logger.debug("pushing process %d back...", p->id);
        logger.debug("processes vector size = %u", processes.size());
        logger.debug("processes vector capacity = %u", processes.capacity());
        logger.debug("p->id = %d", p->id);
        logger.debug("p->rsp = 0x%x", p->rsp);
        logger.debug("p->rip = 0x%x", p->rip);
        lib::debug_new = true;
        processes.push_back(p, true);
        lib::debug_new = false;
        logger.debug("checking current process...");
        if (!curr_proc)
                curr_proc = p;
}
// --------------------------------------------------

// --------------------------------------------------
void tick(cpu::IRQFrame *frame)
{
        if (!active || processes.size() == 0 || !curr_proc)
                return;

        curr_proc->time++;
        if (curr_proc->time < TIME_PER_PROCESS)
                return;
        curr_proc->time = 0;

        curr_proc_idx = (curr_proc_idx + 1) % processes.size();

        Process *old_proc = curr_proc;
        Process *new_proc = processes[curr_proc_idx];

        if (old_proc)
                old_proc->save_context(frame);
        new_proc->load_context(frame);

        curr_proc = new_proc;
        new_proc->load();
}
// --------------------------------------------------

// --------------------------------------------------
bool is_active()
{
        return active;
}
// --------------------------------------------------

// --------------------------------------------------
Process *get_current_process()
{
        return curr_proc;
}
// --------------------------------------------------

// --------------------------------------------------
void set_current_process(Process *p)
{
        curr_proc = p;
}
// --------------------------------------------------

} /* namespace kernel::proc::scheduler */