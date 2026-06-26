#include "mem/pmm.hpp"
#include <cpu/userspace.hpp>
#include <kernel.hpp>
#include <lib/logging.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <mem/vmm.hpp>
#include <proc/elf.hpp>
#include <proc/exec.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>

using kernel::lib::String;
using kernel::lib::u64, kernel::lib::uptr;
using kernel::lib::log::logger;

namespace kernel::proc {

// --------------------------------------------------
int exec(const lib::String &path)
{
        // create the process' pml4t
        u64 *kpml4t = get_kernel_pml4t();
        u64 *proc_pml4t = mem::vmm::create_pml4t(kpml4t);

        // load the file (assume ELF64)
        uptr proc_addr = 0;
        int load_res = elf::load_elf(proc_pml4t, path, &proc_addr);
        if (load_res != 0) {
                logger.err("failed to execute process: failed to load %s", path.raw());
                return -1;
        }

        // create the process
        void (*proc_entry)() = reinterpret_cast<void (*)()>(proc_addr);
        Process proc(allocate_pid(), proc_entry, proc_pml4t);

        // tell the scheduler that the process exists
        scheduler::add_process(&proc);

        scheduler::set_current_process(&proc);
        Process *p = scheduler::get_current_process();
        mem::vmm::map_page(p->pml4t, 
                cpu::USER_STACK_TOP, 
                mem::pmm::allocate_frame(),
                mem::vmm::PageFlag::ReadWriteUser | mem::vmm::PageFlag::NoExec
        );
        p->load();
        logger.debug("preparing to jump at 0x%x", p->entry);
        cpu::enter_userspace(
                reinterpret_cast<void *>(p->entry),
                reinterpret_cast<void *>(cpu::USER_STACK_TOP - 8)
        );

        return 0;
}
// --------------------------------------------------

} /* namespace kernel::proc */