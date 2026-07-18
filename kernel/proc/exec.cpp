#include <cpu/assembly.hpp>
#include <cpu/userspace.hpp>
#include <kernel.hpp>
#include <lib/logging.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <proc/elf.hpp>
#include <proc/exec.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>
#include <panic.hpp>

using kernel::lib::String;
using kernel::lib::u64, kernel::lib::uptr;
using kernel::lib::log::logger;

namespace kernel::proc {

Process *load_as_proc(const String &path)
{
        // create the process' pml4t
        u64 *kpml4t = get_kernel_pml4t();
        u64 *proc_pml4t = mem::vmm::create_pml4t(kpml4t);

        // load the file (assume ELF64)
        uptr proc_addr = 0;
        int load_res = elf::load_elf(proc_pml4t, path, &proc_addr);
        if (load_res != 0)
                return nullptr; // could not load executable

        // create the process
        void (*proc_entry)() = reinterpret_cast<void (*)()>(proc_addr);
        Process *proc = new Process(allocate_pid(), proc_entry, proc_pml4t);

        return proc;
}

// --------------------------------------------------
int spawn(const String &path)
{
        cpu::cli();

        Process *proc = load_as_proc(path);
        if (!proc) {
                logger.err("failed to spawn process: failed to load %s", path.raw());
                return -1;
        }

        // tell the scheduler that the process exists
        scheduler::add_process(proc);

        scheduler::set_current_process(proc);
        Process *p = scheduler::get_current_process();
        p->load();

        if (!p->entry)
                panic("failed to spawn process: null entry point");

        logger.debug("entering userspace...");
        cpu::enter_userspace(
                reinterpret_cast<void *>(p->rip),
                reinterpret_cast<void *>(p->rsp)
        );

        cpu::sti();

        return 0;
}
// --------------------------------------------------

int exec(const lib::String &path)
{
        cpu::cli();

        Process *proc = scheduler::get_current_process();

        u64 *kpml4t = get_kernel_pml4t();
        u64 *proc_pml4t = mem::vmm::create_pml4t(kpml4t);

        // load the file (assume ELF64)
        uptr proc_addr = 0;
        int load_res = elf::load_elf(proc_pml4t, path, &proc_addr);
        if (load_res != 0)
                return -1; // could not load executable

        // create the process
        void (*proc_entry)() = reinterpret_cast<void (*)()>(proc_addr);

        mem::vmm::destroy_pml4t(proc->pml4t);
        proc->pml4t = proc_pml4t;

        proc->rsp = cpu::USER_STACK_TOP;
        for (uptr addr = cpu::USER_STACK_BOTTOM; addr < cpu::USER_STACK_TOP; addr += mem::vmm::PAGE_BYTES) {
                mem::vmm::map_page(proc->pml4t,
                        addr,
                        mem::pmm::allocate_frame(),
                        mem::vmm::PageFlag::ReadWriteUser | mem::vmm::PageFlag::NoExec
                );
        }

        proc->entry = proc_entry;
        proc->flags = 1 << 9;
        proc->rip = reinterpret_cast<u64>(proc_entry);

        proc->load();

        cpu::sti();

        return 0;
}

} /* namespace kernel::proc */