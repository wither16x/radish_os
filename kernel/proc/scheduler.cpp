#include <kernel.hpp>
#include <mem/pmm.hpp>
#include <cpu/gdt.hpp>
#include <cpu/assembly.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>
#include <lib/queue.hpp>
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

        mem::pmm::free_frame(zombie->kstack_frame);
        remove_process(zombie->id);
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

void remove_process(PID pid)
{
        for (usize i = 0; i < ctx.processes.size(); i++) {
                if (ctx.processes[i]->id == pid) {
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

        if (ctx.current_process->status != ProcessStatus::Dead) {
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
                if (ctx.processes[idx]->status != ProcessStatus::Dead) {
                        new_proc = ctx.processes[idx];
                        new_proc_idx = idx;
                        break;
                }
        }

        if (!new_proc) {
                ctx.current_process = nullptr;
                if (old_proc->status == ProcessStatus::Dead)
                        undertaker(old_proc->id);
                return;
        }

        if (new_proc == old_proc)
                return;

        ctx.current_process = new_proc;
        ctx.current_process_index = new_proc_idx;

        new_proc->load_pml4t();
        get_kernel_gdt().get_tss().reset_stack(new_proc->kstack_top);

        if (old_proc->status == ProcessStatus::Dead) {
                uptr discard = 0;
                ctx.pending_zombie = old_proc;
                proc_switch(&discard, new_proc->krsp);
                while (true)
                        cpu::hlt();
        }

        proc_switch(&old_proc->krsp, new_proc->krsp);
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
                if (proc->id == pid)
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
void undertaker(PID pid)
{
        Process *proc = get_process_by_id(pid);
        if (!proc) {
                logger.err("undertaker: process %d does not exist", pid);
                return;
        }

        remove_process(pid);
        proc->pml4t.destroy();
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
                if (ctx.processes[idx]->status != ProcessStatus::Dead && ctx.processes[idx] != old_proc) {
                        new_proc = ctx.processes[idx];
                        ctx.current_process_index = idx;
                        break;
                }
        }

        if (!new_proc)
                return;

        new_proc->load_pml4t();
        get_kernel_gdt().get_tss().reset_stack(new_proc->kstack_top);
        ctx.current_process = new_proc;

        proc_switch(&old_proc->krsp, new_proc->krsp);
}

} /* namespace kernel::proc::scheduler */