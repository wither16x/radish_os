#include "cpu/cpu.hpp"
#include <proc/fork.hpp>
#include <proc/scheduler.hpp>
#include <proc/process.hpp>
#include <proc/pid.hpp>
#include <mem/pml4t.hpp>
#include <cpu/assembly.hpp>

namespace Kiwi::Proc
{
        pid_t fork()
        {
                Cpu::disableInterrupts();

                Process *parent = Scheduler::getCurrentProcess();

                Mem::PML4T child_pml4t;
                child_pml4t.init(parent->getPml4t());

                Process *child = new Process(allocatePid(), *parent, child_pml4t);

                parent->addChild(child);
                Scheduler::addProcess(child);

                Cpu::enableInterrupts();
                return child->getId();
        }
} // namespace Kiwi::Proc