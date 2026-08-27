#include "cpu/cpu.hpp"
#include <mem/pmm.hpp>
#include <cpu/gdt.hpp>
#include <cpu/assembly.hpp>
#include <lib/logging.hpp>
#include <lib/memory.hpp>
#include <lib/typing.hpp>
#include <lib/filesystem.hpp>
#include <proc/scheduler.hpp>
#include <proc/process.hpp>

namespace Kiwi::Proc::Scheduler
{
        namespace
        {
                struct SchedulerContext
                {
                        Status status;
                        Lib::Vector<Process *> processes;
                        Process *current_process;
                        Lib::usize current_process_index;
                        Process *pending_zombie;
                };

                SchedulerContext ctx;

                void reapPendingZombie()
                {
                        if (not ctx.pending_zombie)
                                return;

                        Process *zombie = ctx.pending_zombie;
                        ctx.pending_zombie = nullptr;

                        for (auto &fd : zombie  ->getFileDescriptors())
                                Lib::close(fd);

                        Mem::Pmm::freeFrame(zombie->getKernelStack().getFrame());
                        removeProcess(zombie);
                        delete zombie;
                }

                Process *findNextRunnableProcess(Process *old_proc, Lib::usize *new_proc_idx)
                {
                        Lib::usize __new_proc_idx = ctx.current_process_index;

                        for (Lib::usize i = 0; i < ctx.processes.size(); i++) {
                                Lib::usize idx = (ctx.current_process_index + 1 + i) % ctx.processes.size();
                                if (not ctx.processes[idx]->isDead() and ctx.processes[idx] != old_proc) {
                                        __new_proc_idx = idx;
                                        Lib::memcpy(new_proc_idx, &__new_proc_idx, sizeof(*new_proc_idx));
                                        return ctx.processes[idx];
                                }
                        }

                        return nullptr;
                }
        } // anonymous namespace

        void init()
        {
                ctx.current_process = 0;
                ctx.current_process = nullptr;
                ctx.status = Status::Unlocked;

                Lib::Log::logger.ok("initialized scheduler");
        }

        void addProcess(Process *p)
        {
                // this function only adds the process to the Lib::Vector<typename T>;
                // it must have been initialized before
                ctx.processes.pushBack(p);
                if (not ctx.current_process)
                        ctx.current_process = p;
        }

        // void remove_process(PID pid)
        void removeProcess(Process *p)
        {
                for (Lib::usize i = 0; i < ctx.processes.size(); i++) {
                        if (ctx.processes[i] == p) {
                                ctx.processes.erase(i);
                                break;
                        }
                }
        }

        void tick()
        {
                if (ctx.status == Status::Locked or ctx.processes.empty() or not ctx.current_process)
                        return;

                if (ctx.current_process->getStatus() != ProcessStatus::Dead) {
                        ctx.current_process->consumeTime(1);
                        if (ctx.current_process->getTime() < TIME_PER_PROCESS)
                                return;
                        ctx.current_process->resetTime();
                }

                Process *old_proc = ctx.current_process;
                Lib::usize new_proc_idx = 0;
                Process *new_proc = findNextRunnableProcess(nullptr, &new_proc_idx);

                if (not new_proc) {
                        ctx.current_process = nullptr;
                        if (old_proc->isDead())
                                undertaker(old_proc);
                        return;
                }

                if (new_proc == old_proc)
                        return;

                ctx.current_process = new_proc;
                ctx.current_process_index = new_proc_idx;

                new_proc->loadPml4t();
                new_proc->useKernelStack();

                if (old_proc->isDead()) {
                        Lib::uptr discard = 0;
                        ctx.pending_zombie = old_proc;
                        __proc_switch(&discard, new_proc->getKernelStack().get());
                        while (true)
                                Cpu::idle();
                }

                old_proc->switchWith(new_proc);
                reapPendingZombie();
        }

        bool isActive()
        {
                return ctx.status != Status::Locked;
        }

        Process *getCurrentProcess()
        {
                return ctx.current_process;
        }

        Process *getProcessById(pid_t pid)
        {
                for (auto &proc : ctx.processes) {
                        if (proc->getId() == pid)
                                return proc;
                }

                return nullptr; // process not found
        }

        void setCurrentProcess(Process *p)
        {
                ctx.current_process = p;
        }

        const Lib::Vector<Process *> &getProcesses()
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
                if (not p) {
                        Lib::Log::logger.err("undertaker: process does not exist");
                        return;
                }

                for (auto &fd : p->getFileDescriptors())
                        Lib::close(fd);

                Mem::Pmm::freeFrame(p->getKernelStack().getFrame());
                removeProcess(p);
                delete p;
        }

        void yield()
        {
                Process *old_proc = ctx.current_process;
                if (not old_proc) {
                        Lib::Log::logger.debug("old process is null");
                        return;
                }

                Lib::usize start_idx = 0;
                Process *new_proc = findNextRunnableProcess(old_proc, &start_idx);

                if (not new_proc)
                        return;

                new_proc->loadPml4t();
                new_proc->useKernelStack();
                ctx.current_process = new_proc;

                old_proc->switchWith(new_proc);
        }

        void lock()
        {
                ctx.status = Status::Locked;
        }

        void unlock()
        {
                ctx.status = Status::Unlocked;
        }
} // namespace Kiwi::Proc::Scheduler