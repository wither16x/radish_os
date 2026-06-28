#include <cpu/irq.hpp>
#include <lib/logging.hpp>
#include <lib/time.hpp>
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
        processes.push_back(p);
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

        if (old_proc) {
                old_proc->rax   = frame->rax;
                old_proc->rbx   = frame->rbx;
                old_proc->rcx   = frame->rcx;
                old_proc->rdx   = frame->rdx;
                old_proc->rsi   = frame->rsi;
                old_proc->rdi   = frame->rdi;
                old_proc->rbp   = frame->rbp;
                old_proc->r8    = frame->r8;
                old_proc->r9    = frame->r9;
                old_proc->r10   = frame->r10;
                old_proc->r11   = frame->r11;
                old_proc->r12   = frame->r12;
                old_proc->r13   = frame->r13;
                old_proc->r14   = frame->r14;
                old_proc->r15   = frame->r15;
                old_proc->rip   = frame->rip;
                old_proc->cs    = frame->cs;
                old_proc->flags = frame->flags | (1 << 9);
                old_proc->rsp   = frame->rsp;
                old_proc->ss    = frame->ss;
                old_proc->cr2   = frame->cr2;
                old_proc->cr3   = frame->cr3;
        }

        frame->rax      = new_proc->rax;
        frame->rbx      = new_proc->rbx;
        frame->rcx      = new_proc->rcx;
        frame->rdx      = new_proc->rdx;
        frame->rsi      = new_proc->rsi;
        frame->rdi      = new_proc->rdi;
        frame->rbp      = new_proc->rbp;
        frame->r8       = new_proc->r8;
        frame->r9       = new_proc->r9;
        frame->r10      = new_proc->r10;
        frame->r11      = new_proc->r11;
        frame->r12      = new_proc->r12;
        frame->r13      = new_proc->r13;
        frame->r14      = new_proc->r14;
        frame->r15      = new_proc->r15;
        frame->rip      = new_proc->rip;
        frame->cs       = new_proc->cs;
        frame->flags    = new_proc->flags | (1 << 9);
        frame->rsp      = new_proc->rsp;
        frame->ss       = new_proc->ss;
        frame->cr2      = new_proc->cr2;
        frame->cr3      = new_proc->cr3;

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