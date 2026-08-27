#include "cpu/userspace.hpp"
#include <cpu/cpu.hpp>
#include <proc/procheap.hpp>
#include <lib/typing.hpp>
#include <mem/pmm.hpp>
#include <mem/page.hpp>

namespace Kiwi::Proc
{
        ProcessHeap::ProcessHeap(Lib::uptr start, Lib::uptr last_page, Lib::uptr limit, Mem::PML4T &pml4t)
                : start(start), last_page(last_page), limit(limit), pml4t(pml4t)
        {}

        bool ProcessHeap::extend(this ProcessHeap &self, int pages)
        {
                Cpu::disableInterrupts();

                if (self.last_page + pages * Mem::PAGE_SIZE > self.limit) {
                        Cpu::enableInterrupts();
                        return false;
                }

                Lib::uptr vaddr = self.last_page;
                for (int i = 0; i < pages; i++) {
                        self.pml4t.mapPage(vaddr,
                                Mem::Pmm::allocateFrame(),
                                Mem::PageFlag::NoExec | Mem::PageFlag::ReadWriteUser
                        );
                        vaddr += Mem::PAGE_SIZE;
                }

                self.last_page += pages * Mem::PAGE_SIZE;
                Cpu::enableInterrupts();
                return true;
        }

        bool ProcessHeap::shorten(this ProcessHeap &self, int pages)
        {
                Cpu::disableInterrupts();

                if (self.last_page - pages * Mem::PAGE_SIZE < self.start) {
                        Cpu::enableInterrupts();
                        return false;
                }

                Lib::uptr vaddr = self.last_page - Mem::PAGE_SIZE;
                for (int i = 0; i < pages; i++) {
                        self.pml4t.unmapPage(vaddr);
                        vaddr -= Mem::PAGE_SIZE;
                }

                self.last_page -= pages * Mem::PAGE_SIZE;
                Cpu::enableInterrupts();
                return true;
        }

        void ProcessHeap::reset(this ProcessHeap &self, Lib::uptr new_start, Mem::PML4T &pml4t)
        {
                Lib::uptr aligned = Mem::pageAlignUp(new_start);
                self.start = aligned;
                self.last_page = aligned;
                self.limit = Cpu::USER_HEAP_LIMIT;
                self.pml4t = pml4t;
        }

        Lib::uptr ProcessHeap::getStart(this const ProcessHeap &self)
        {
                return self.start;
        }

        Lib::uptr ProcessHeap::getLastPage(this const ProcessHeap &self)
        {
                return self.last_page;
        }

        Lib::uptr ProcessHeap::getLimit(this const ProcessHeap &self)
        {
                return self.limit;
        }
} // namespace Kiwi::Proc