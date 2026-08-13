#include <cpu/cpu.hpp>
#include <kernel.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/memory.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <mem/pml4t.hpp>
#include <proc/elf.hpp>
#include <proc/exec.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>
#include <panic.hpp>

using kernel::lib::String;
using kernel::lib::u64, kernel::lib::uptr;

namespace kernel::proc {

int exec(const String &path, int argc, char **argv, char **envp)
{
        cpu::disable_interrupts();

        // We want to update the current process
        Process *proc = scheduler::get_current_process();

        // Load the program located at `path`
        mem::PML4T &kpml4t = get_kernel_pml4t();
        mem::PML4T proc_pml4t;
        proc_pml4t.init(kpml4t);
        elf::ElfInfo elf_info;
        int load_res = elf::load_elf(&proc_pml4t, path, &elf_info);
        if (load_res != 0) {
                cpu::enable_interrupts();
                return -1;
        }

        elf::elf_entry_t proc_entry = reinterpret_cast<elf::elf_entry_t>(elf_info.address);

        if (proc->init_arguments(argc, argv, envp) != 0)
                logger.err("failed to initialize process arguments");

        // Update the process
        // should add a dedicated method
        proc->reset_fpu_context();
        proc->switch_pml4t(proc_pml4t);
        proc->reset_stack();
        proc->init_user_stack();
        proc->switch_entry(proc_entry);
        proc->load_pml4t();

        cpu::enable_interrupts();

        return 0;
}

} /* namespace kernel::proc */