#include <cpu/cpu.hpp>
#include <kernel.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/memory.hpp>
#include <lib/logging.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <mem/pml4t.hpp>
#include <proc/elf.hpp>
#include <proc/exec.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>
#include <panic.hpp>

namespace Kiwi::Proc
{
        int exec(const Lib::String &path, int argc, char **argv, char **envp)
        {
                Cpu::disableInterrupts();

                // We want to update the current process
                Process *proc = Scheduler::getCurrentProcess();

                // Load the program located at `path`
                Mem::PML4T &kpml4t = getKernelPml4t();
                Mem::PML4T proc_pml4t;
                proc_pml4t.init(kpml4t);
                Elf::ElfInfo elf_info;
                int load_res = Elf::loadElf(&proc_pml4t, path, &elf_info);
                if (load_res != 0) {
                        Cpu::enableInterrupts();
                        return -1;
                }

                Elf::elf_entry_t proc_entry = reinterpret_cast<Elf::elf_entry_t>(elf_info.address);

                if (proc->initArguments(argc, argv, envp) != 0)
                        Lib::Log::logger.err("failed to initialize process arguments");

                // Update the process
                // should add a dedicated method
                proc->resetFpuContext();
                proc->switchPml4t(proc_pml4t);
                proc->resetHeap(elf_info.highest_vaddr);
                proc->resetStack();
                proc->initUserStack();
                proc->switchEntry(proc_entry);
                proc->loadPml4t();

                Cpu::enableInterrupts();

                return 0;
        }
} // namespace Kiwi::Proc