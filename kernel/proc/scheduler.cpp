#include <kernel.hpp>
#include <mem/pmm.hpp>
#include <cpu/gdt.hpp>
#include <cpu/assembly.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>
#include <proc/scheduler.hpp>
#include <proc/process.hpp>

using kernel::lib::u64, kernel::lib::usize, kernel::lib::uptr;
using kernel::lib::log::logger;
using kernel::lib::Vector;

namespace kernel::proc::scheduler {

namespace {

struct SchedulerContext {
        bool is_active;

        Vector<Process *> processes;

        Process *current_process;
        usize current_process_index;

        Process *pending_zombie;
};

SchedulerContext ctx;

void reap_pending_zombie()
{
        if (!ctx.pending_zombie)
                return;

        Process *zombie = ctx.pending_zombie;
        ctx.pending_zombie = nullptr;

        mem::pmm::free_frame(zombie->kernel_stack_frame());
        remove_process(zombie);
        delete zombie;
}

} /* anonymous namespace */

// --------------------------------------------------
void init()
{
        ctx.current_process = 0;
        ctx.current_process = nullptr;
        ctx.is_active = true;

        logger.ok("initialized scheduler");
}
// --------------------------------------------------

// --------------------------------------------------
void add_process(Process *p)
{
        // this function only adds the process to the vector;
        // it must have been initialized before
        ctx.processes.push_back(p);
        if (!ctx.current_process)
                ctx.current_process = p;
}
// --------------------------------------------------

// void remove_process(PID pid)
void remove_process(Process *p)
{
        for (usize i = 0; i < ctx.processes.size(); i++) {
                if (ctx.processes[i] == p) {
                        ctx.processes.erase(i);
                        break;
                }
        }
}

// --------------------------------------------------
void tick()
{
        if (!ctx.is_active || ctx.processes.empty() || !ctx.current_process)
                return;

        if (ctx.current_process->get_status() != ProcessStatus::Dead) {
                ctx.current_process->consume_time(1);
                if (ctx.current_process->get_time() < TIME_PER_PROCESS)
                        return;
                ctx.current_process->reset_time();
        }

        Process *old_proc = ctx.current_process;
        Process *new_proc = nullptr;
        usize new_proc_idx = ctx.current_process_index;

        for (usize i = 0; i < ctx.processes.size(); i++) {
                usize idx = (ctx.current_process_index + 1 + i) % ctx.processes.size();
                if (!ctx.processes[idx]->is_dead()) {
                        new_proc = ctx.processes[idx];
                        new_proc_idx = idx;
                        break;
                }
        }

        if (!new_proc) {
                ctx.current_process = nullptr;
                if (old_proc->is_dead())
                        undertaker(old_proc);
                return;
        }

        if (new_proc == old_proc)
                return;

        ctx.current_process = new_proc;
        ctx.current_process_index = new_proc_idx;

        new_proc->load_pml4t();
        get_kernel_gdt().get_tss().reset_stack(new_proc->kernel_stack_top());

        if (old_proc->is_dead()) {
                uptr discard = 0;
                ctx.pending_zombie = old_proc;
                __proc_switch(&discard, new_proc->kernel_stack_pointer());
                while (true)
                        cpu::hlt();
        }

        old_proc->switch_with(new_proc);
        reap_pending_zombie();
}
// --------------------------------------------------

// --------------------------------------------------
bool is_active()
{
        return ctx.is_active;
}
// --------------------------------------------------

// --------------------------------------------------
Process *get_current_process()
{
        return ctx.current_process;
}
// --------------------------------------------------

Process *get_process_by_id(PID pid)
{
        for (auto &proc : ctx.processes) {
                if (proc->get_id() == pid)
                        return proc;
        }

        return nullptr; // process not found
}

// --------------------------------------------------
void set_current_process(Process *p)
{
        ctx.current_process = p;
}
// --------------------------------------------------

const Vector<Process *> &get_processes()
{
        return ctx.processes;
}

/// The undertaker assumes that the process is NOT using the
/// CPU.
/// Step 1: remove the dead process from the scheduler
/// Step 2: destroy the process' page tables
/// Step 3: destroy the process itself
void undertaker(Process *p)
{
        if (!p) {
                logger.err("undertaker: process does not exist");
                return;
        }

        remove_process(p);
        p->destroy_pml4t();
}

void yield()
{
        Process *old_proc = ctx.current_process;
        if (!old_proc) {
                logger.debug("old process is null");
                return;
        }

        Process *new_proc = nullptr;
        usize start_idx = ctx.current_process_index;

        for (usize i = 0; i < ctx.processes.size(); i++) {
                usize idx = (start_idx + 1 + i) % ctx.processes.size();
                if (!ctx.processes[idx]->is_dead() && ctx.processes[idx] != old_proc) {
                        new_proc = ctx.processes[idx];
                        ctx.current_process_index = idx;
                        break;
                }
        }

        if (!new_proc)
                return;

        new_proc->load_pml4t();
        get_kernel_gdt().get_tss().reset_stack(new_proc->kernel_stack_top());
        ctx.current_process = new_proc;

        old_proc->switch_with(new_proc);
}

} /* namespace kernel::proc::scheduler */