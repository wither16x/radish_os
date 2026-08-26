#pragma once

#include <lib/typing.hpp>
#include <lib/vector.hpp>
#include <proc/process.hpp>
#include <proc/pid.hpp>

namespace Kiwi::Proc::Scheduler
{
        constexpr Lib::u64 TIME_PER_PROCESS = 35; // in ms

        enum class Status
        {
                Locked,
                Unlocked
        };

        /// Initialize the scheduler.
        void init();
        /// Add a process to the scheduler's process vector.
        void addProcess(Process *p);
        /// Remove a process from the scheduler's process vector.
        void removeProcess(Process *p);
        /// Try to switch processes.
        void tick();
        /// Check if the scheduler is active/has been initialized.
        bool isActive();
        /// Return the currently active process.
        Process *getCurrentProcess();
        /// Return any process or nullptr by its ID.
        Process *getProcessById(pid_t pid);
        /// Set the currently active process.
        void setCurrentProcess(Process *p);
        /// Return the scheduler's process vector.
        const Lib::Vector<Process *> &getProcesses();
        /// Clean the process with the ID `pid` if it is dead.
        void undertaker(Process *p);
        /// Let other processes use the CPU. Use this to avoid locking the CPU.
        void yield();
        void lock();
        void unlock();
} // namespace Kiwi::Proc::Scheduler