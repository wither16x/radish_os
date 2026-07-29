#include <proc/spawn.hpp>
#include <proc/process.hpp>
#include <proc/elf.hpp>
#include <proc/scheduler.hpp>
#include <proc/pid.hpp>
#include <kernel.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <cpu/assembly.hpp>
#include <cpu/userspace.hpp>
#include <panic.hpp>

using kernel::lib::String;
using kernel::lib::uptr;

namespace kernel::proc {

namespace {

Process *load_program_as_process(const String &path)
{
        // create the process' pml4t
        mem::PML4T &kpml4t = get_kernel_pml4t();
        mem::PML4T proc_pml4t;
        proc_pml4t.init(kpml4t);

        // load the file (assume ELF64)
        elf::ElfInfo elf_info;
        int load_res = elf::load_elf(&proc_pml4t, path, &elf_info);
        if (load_res != 0)
                return nullptr; // could not load executable

        elf_info.entry = reinterpret_cast<void (*)()>(elf_info.address);

        // create the process
        Process *proc = new Process(allocate_pid(), &elf_info, proc_pml4t);

        return proc;
}

} /* anonymous namespace */

// --------------------------------------------------
int spawn(const String &path)
{
        cpu::cli();

        Process *proc = load_program_as_process(path);
        if (!proc) {
                logger.err("failed to spawn process: failed to load %s", path.raw());
                return -1;
        }

        // tell the scheduler that the process exists
        scheduler::add_process(proc);

        scheduler::set_current_process(proc);
        Process *p = scheduler::get_current_process();
        p->load_pml4t();

        if (!p->get_entry())
                panic("failed to spawn process: null entry point");

        cpu::enter_userspace(
                reinterpret_cast<void *>(p->get_stack_frame()->rip),
                reinterpret_cast<void *>(p->get_stack_frame()->rsp)
        );

        cpu::sti();

        return 0;
}
// --------------------------------------------------

} /* namespace kernel::proc */