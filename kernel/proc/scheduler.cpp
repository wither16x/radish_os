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

Vector<Process *> processes;
Process *curr_proc = nullptr;
Process *pending_zombie = nullptr;
usize curr_proc_idx = 0;
bool active = false;

void reap_pending_zombie()
{
        if (!pending_zombie)
                return;

        Process *zombie = pending_zombie;
        pending_zombie = nullptr;

        mem::pmm::free_frame(zombie->kstack_frame);
        remove_process(zombie->id);
        delete zombie;
}

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
        processes.push_back(p);
        if (!curr_proc)
                curr_proc = p;
}
// --------------------------------------------------

void remove_process(PID pid)
{
        for (usize i = 0; i < processes.size(); i++) {
                if (processes[i]->id == pid) {
                        processes.erase(i);
                        break;
                }
        }
}

// --------------------------------------------------
void tick()
{
        if (!active || processes.size() == 0 || !curr_proc)
                return;

        if (curr_proc->status != ProcessStatus::Dead) {
                curr_proc->time++;
                if (curr_proc->time < TIME_PER_PROCESS)
                        return;
                curr_proc->time = 0;
        }

        Process *old_proc = curr_proc;
        Process *new_proc = nullptr;

        usize new_proc_idx = curr_proc_idx;

        for (usize i = 0; i < processes.size(); i++) {
                usize idx = (curr_proc_idx + 1 + i) % processes.size();
                if (processes[idx]->status != ProcessStatus::Dead) {
                        new_proc = processes[idx];
                        new_proc_idx = idx;
                        break;
                }
        }

        if (!new_proc) {
                curr_proc = nullptr;
                if (old_proc->status == ProcessStatus::Dead)
                        undertaker(old_proc->id);
                return;
        }

        if (new_proc == old_proc)
                return;

        curr_proc = new_proc;
        curr_proc_idx = new_proc_idx;

        new_proc->load_pml4t();
        get_kernel_gdt().get_tss().reset_stack(new_proc->kstack_top);

        if (old_proc->status == ProcessStatus::Dead) {
                uptr discard = 0;
                pending_zombie = old_proc;
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
        return active;
}
// --------------------------------------------------

// --------------------------------------------------
Process *get_current_process()
{
        return curr_proc;
}
// --------------------------------------------------

Process *get_process_by_id(PID pid)
{
        for (auto &proc : processes) {
                if (proc->id == pid)
                        return proc;
        }

        return nullptr; // process not found
}

// --------------------------------------------------
void set_current_process(Process *p)
{
        curr_proc = p;
}
// --------------------------------------------------

const Vector<Process *> &get_processes()
{
        return processes;
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
        Process *old_proc = curr_proc;
        if (!old_proc) {
                logger.debug("old process is null");
                return;
        }

        Process *new_proc = nullptr;
        usize start_idx = curr_proc_idx;

        for (usize i = 0; i < processes.size(); i++) {
                usize idx = (start_idx + 1 + i) % processes.size();
                if (processes[idx]->status != ProcessStatus::Dead && processes[idx] != old_proc) {
                        new_proc = processes[idx];
                        curr_proc_idx = idx;
                        break;
                }
        }

        if (!new_proc)
                return;

        new_proc->load_pml4t();
        get_kernel_gdt().get_tss().reset_stack(new_proc->kstack_top);
        curr_proc = new_proc;

        proc_switch(&old_proc->krsp, new_proc->krsp);
}

} /* namespace kernel::proc::scheduler */