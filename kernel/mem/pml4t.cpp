#include <kernel.hpp>
#include <mem/pml4t.hpp>
#include <mem/pmm.hpp>
#include <mem/heap.hpp>
#include <mem/page.hpp>
#include <lib/typing.hpp>
#include <lib/memory.hpp>
#include <cpu/assembly.hpp>

using kernel::lib::uptr, kernel::lib::u64, kernel::lib::usize, kernel::lib::u16;
using kernel::lib::memset;

namespace kernel::mem {

void PML4T::init(this PML4T &self)
{
        uptr hhdm_offset = get_kernel_hhdm_offset();

        uptr frame = pmm::allocate_frame();
        self.raw_pml4t = reinterpret_cast<PageTable *>(frame + hhdm_offset);
        memset(self.raw_pml4t, 0, PAGE_SIZE);     
}

void PML4T::init(this PML4T &self, const PML4T &parent)
{
        self.init();

        for (usize i = PAGE_TABLE_ENTRIES / 2; i < PAGE_TABLE_ENTRIES; i++)
                self.raw_pml4t->entries[i] = parent.raw()->entries[i];
}

void PML4T::destroy(this PML4T &self)
{
        uptr hhdm_offset = get_kernel_hhdm_offset();

        for (usize pml4t_idx = 0; pml4t_idx < PAGE_TABLE_ENTRIES / 2; pml4t_idx++) {
                if (!(self.raw_pml4t->entries[pml4t_idx] & 1))
                        continue;

                if (pml4t_idx == mem::heap::HEAP_PML4T_IDX / 2)
                        continue;

                PageTable *pdpt = reinterpret_cast<PageTable *>((self.raw_pml4t->entries[pml4t_idx] & PHYS_ADDR_MASK) + hhdm_offset);
                for (u64 pdpt_idx = 0; pdpt_idx < PAGE_TABLE_ENTRIES; pdpt_idx++) {
                        if (!(pdpt->entries[pdpt_idx] & 1))
                                continue;

                        PageTable *pdt = reinterpret_cast<PageTable *>((pdpt->entries[pdpt_idx] & PHYS_ADDR_MASK) + hhdm_offset);
                        for (u16 pdt_idx = 0; pdt_idx < PAGE_TABLE_ENTRIES; pdt_idx++) {
                                if (!(pdt->entries[pdt_idx] & 1))
                                        continue;

                                PageTable *pt = reinterpret_cast<PageTable *>((pdt->entries[pdt_idx] & PHYS_ADDR_MASK) + hhdm_offset);
                                pmm::free_frame(reinterpret_cast<u64>(pt->entries) - hhdm_offset);
                        }

                        pmm::free_frame(reinterpret_cast<u64>(pdt) - hhdm_offset);
                }

                pmm::free_frame(reinterpret_cast<u64>(pdpt) - hhdm_offset);
        }

        pmm::free_frame(reinterpret_cast<u64>(self.raw_pml4t) - hhdm_offset);
}

void PML4T::load(this const PML4T &self)
{
        uptr hhdm_offset = get_kernel_hhdm_offset();

        __asm__ volatile ("movq %0, %%cr3" :: "r"(reinterpret_cast<u64>(self.raw_pml4t) - hhdm_offset));
}

void PML4T::map_page(this PML4T &self, uptr vaddr, uptr paddr, u64 flags)
{
        uptr hhdm_offset = get_kernel_hhdm_offset();

        // create intermediate page tables containing informations
        // that tell the CPU where the corresponding frame is

        // split the virtual address to find the its indices in the
        // page tables
        usize pml4t_idx = (vaddr >> 39) & 0x1ff;
        usize pdpt_idx  = (vaddr >> 30) & 0x1ff;
        usize pdt_idx   = (vaddr >> 21) & 0x1ff;
        usize pt_idx    = (vaddr >> 12) & 0x1ff;

        // now map the page to the given frame
        if (!(self.raw_pml4t->entries[pml4t_idx] & 1)) {
                self.raw_pml4t->entries[pml4t_idx] = pmm::allocate_frame() | PageFlag::ReadWriteUser;
                memset(
                        reinterpret_cast<u64 *>((self.raw_pml4t->entries[pml4t_idx] & PHYS_ADDR_MASK) + hhdm_offset),
                        0,
                        PAGE_SIZE
                );
        }

        PageTable *pdpt = reinterpret_cast<PageTable *>((self.raw_pml4t->entries[pml4t_idx] & PHYS_ADDR_MASK) + hhdm_offset);
        if (!(pdpt->entries[pdpt_idx] & 1)) {
                pdpt->entries[pdpt_idx] = pmm::allocate_frame() | PageFlag::ReadWriteUser;
                memset(
                        reinterpret_cast<u64 *>((pdpt->entries[pdpt_idx] & PHYS_ADDR_MASK) + hhdm_offset),
                        0,
                        PAGE_SIZE
                );
        }

        PageTable *pdt = reinterpret_cast<PageTable *>((pdpt->entries[pdpt_idx] & PHYS_ADDR_MASK) + hhdm_offset);
        if (!(pdt->entries[pdt_idx] & 1)) {
                pdt->entries[pdt_idx] = pmm::allocate_frame() | PageFlag::ReadWriteUser;
                memset(
                        reinterpret_cast<u64 *>((pdt->entries[pdt_idx] & PHYS_ADDR_MASK) + hhdm_offset),
                        0,
                        PAGE_SIZE
                );
        }

        PageTable *pt = reinterpret_cast<PageTable *>((pdt->entries[pdt_idx] & PHYS_ADDR_MASK) + hhdm_offset);
        if (!(pt->entries[pt_idx] & 1))
                pt->entries[pt_idx] = paddr | flags;
}

void PML4T::unmap_page(this PML4T &self, uptr vaddr)
{
        uptr hhdm_offset = get_kernel_hhdm_offset();

        u64 pml4t_idx   = (vaddr >> 39) & 0x1ff;
        u64 pdpt_idx    = (vaddr >> 30) & 0x1ff;
        u64 pdt_idx     = (vaddr >> 21) & 0x1ff;
        u64 pt_idx      = (vaddr >> 12) & 0x1ff;

        if (!(self.raw_pml4t->entries[pml4t_idx] & 1))
                return;

        PageTable *pdpt = reinterpret_cast<PageTable *>((self.raw_pml4t->entries[pml4t_idx] & PHYS_ADDR_MASK) + hhdm_offset);
        if (!(pdpt->entries[pdpt_idx] & 1))
                return;

        PageTable *pdt = reinterpret_cast<PageTable *>((pdpt->entries[pdpt_idx] & PHYS_ADDR_MASK) + hhdm_offset);
        if (!(pdt->entries[pdt_idx] & 1))
                return;

        PageTable *pt = reinterpret_cast<PageTable *>((pdt->entries[pdt_idx] & PHYS_ADDR_MASK) + hhdm_offset);
        if (!(pt->entries[pt_idx] & 1))
                return;

        pt->entries[pt_idx] = 0;
        cpu::invlpg(vaddr);

        // A table must have no mapped entries to be deleted

        for (u16 i = 0; i < PAGE_TABLE_ENTRIES; i++) {
                if (pt->entries[i] & 1)
                        return;
        }
        pmm::free_frame(pdt->entries[pdt_idx] & PHYS_ADDR_MASK);
        pdt->entries[pdt_idx] = 0;

        for (u16 i = 0; i < PAGE_TABLE_ENTRIES; i++) {
                if (pdt->entries[i] & 1)
                        return;
        }
        pmm::free_frame(pdpt->entries[pdpt_idx] & PHYS_ADDR_MASK);
        pdpt->entries[pdpt_idx] = 0;

        for (u16 i = 0; i < PAGE_TABLE_ENTRIES; i++) {
                if (pdpt->entries[i] & 1)
                        return;
        }
        pmm::free_frame(self.raw_pml4t->entries[pml4t_idx] & PHYS_ADDR_MASK);
        self.raw_pml4t->entries[pml4t_idx] = 0;
}

PageTable *PML4T::raw(this const PML4T &self)
{
        return self.raw_pml4t;
}

} /* namespace kernel::mem */