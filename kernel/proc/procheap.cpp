#include <cpu/cpu.hpp>
#include <proc/procheap.hpp>
#include <lib/typing.hpp>
#include <mem/pmm.hpp>

using kernel::lib::uptr;

namespace kernel::proc {

ProcessHeap::ProcessHeap(uptr start, uptr last_page, uptr limit, mem::PML4T &pml4t)
        : start(start), last_page(last_page), limit(limit), pml4t(pml4t)
{}

bool ProcessHeap::extend(this ProcessHeap &self, int pages)
{
        cpu::disable_interrupts();

        if (self.last_page + pages * mem::PAGE_SIZE > self.limit) {
                cpu::enable_interrupts();
                return false;
        }

        uptr vaddr = self.last_page;
        for (int i = 0; i < pages; i++) {
                self.pml4t.map_page(vaddr,
                        mem::pmm::allocate_frame(),
                        mem::PageFlag::NoExec | mem::PageFlag::ReadWriteUser
                );
                vaddr += mem::PAGE_SIZE;
        }

        self.last_page += pages * mem::PAGE_SIZE;
        cpu::enable_interrupts();
        return true;
}

bool ProcessHeap::shorten(this ProcessHeap &self, int pages)
{
        cpu::disable_interrupts();

        if (self.last_page - pages * mem::PAGE_SIZE < self.start) {
                cpu::enable_interrupts();
                return false;
        }

        uptr vaddr = self.last_page - mem::PAGE_SIZE;
        for (int i = 0; i < pages; i++) {
                self.pml4t.unmap_page(vaddr);
                vaddr -= mem::PAGE_SIZE;
        }

        self.last_page -= pages * mem::PAGE_SIZE;
        cpu::disable_interrupts();
        return true;
}

uptr ProcessHeap::get_start(this const ProcessHeap &self)
{
        return self.start;
}

uptr ProcessHeap::get_last_page(this const ProcessHeap &self)
{
        return self.last_page;
}

uptr ProcessHeap::get_limit(this const ProcessHeap &self)
{
        return self.limit;
}

} // namespace kernel::proc