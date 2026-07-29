#include <cpu/assembly.hpp>
#include <kernel.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>
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

int exec(const lib::String &path)
{
        cpu::cli();

        // We want to update the current process
        Process *proc = scheduler::get_current_process();

        // Load the program located at `path`
        mem::PML4T &kpml4t = get_kernel_pml4t();
        mem::PML4T proc_pml4t;
        proc_pml4t.init(kpml4t);
        elf::ElfInfo elf_info;
        int load_res = elf::load_elf(&proc_pml4t, path, &elf_info);
        if (load_res != 0) {
                cpu::sti();
                return -1;
        }

        void (*proc_entry)() = reinterpret_cast<void (*)()>(elf_info.address);

        // Update the process
        proc->switch_pml4t(proc_pml4t);
        proc->reset_stack();
        proc->remap_stack();
        proc->switch_entry(proc_entry);
        proc->load_pml4t();

        cpu::sti();

        return 0;
}

} /* namespace kernel::proc */