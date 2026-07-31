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
        uptr hhdm_offset = get_kernel_hhdm_offset();

        self.init();

        for (usize i = PAGE_TABLE_ENTRIES / 2; i < PAGE_TABLE_ENTRIES; i++)
                self.raw_pml4t->entries[i] = parent.raw()->entries[i];

        for (usize i = 0; i < PAGE_TABLE_ENTRIES / 2; i++) {
                u64 entry = parent.raw()->entries[i];
                if (not (entry & PageFlag::Present))
                        continue;

                u64 flags = entry & ~PHYS_ADDR_MASK;
                uptr pdpt_phys = entry & PHYS_ADDR_MASK;
                PageTable *pdpt = reinterpret_cast<PageTable *>(pdpt_phys + hhdm_offset);

                uptr new_pdpt_phys = pt_deep_copy(pdpt, 3);
                self.raw_pml4t->entries[i] = new_pdpt_phys | flags;
        }
}

void PML4T::destroy(this PML4T &self)
{
        uptr hhdm_offset = get_kernel_hhdm_offset();

        for (usize pml4t_idx = 0; pml4t_idx < PAGE_TABLE_ENTRIES / 2; pml4t_idx++) {
                if (not (self.raw_pml4t->entries[pml4t_idx] & PageFlag::Present))
                        continue;

                if (pml4t_idx == mem::heap::HEAP_PML4T_IDX / 2)
                        continue;

                PageTable *pdpt = reinterpret_cast<PageTable *>((self.raw_pml4t->entries[pml4t_idx] & PHYS_ADDR_MASK) + hhdm_offset);
                for (u64 pdpt_idx = 0; pdpt_idx < PAGE_TABLE_ENTRIES; pdpt_idx++) {
                        if (not (pdpt->entries[pdpt_idx] & PageFlag::Present))
                                continue;

                        PageTable *pdt = reinterpret_cast<PageTable *>((pdpt->entries[pdpt_idx] & PHYS_ADDR_MASK) + hhdm_offset);
                        for (u16 pdt_idx = 0; pdt_idx < PAGE_TABLE_ENTRIES; pdt_idx++) {
                                if (not (pdt->entries[pdt_idx] & PageFlag::Present))
                                        continue;

                                PageTable *pt = reinterpret_cast<PageTable *>((pdt->entries[pdt_idx] & PHYS_ADDR_MASK) + hhdm_offset);
                                for (u64 pt_idx = 0; pt_idx < PAGE_TABLE_ENTRIES; pt_idx++) {
                                        if (not (pt->entries[pt_idx] & PageFlag::Present))
                                                continue;

                                        pmm::free_frame(pt->entries[pt_idx] & PHYS_ADDR_MASK);
                                }
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
        if (not (self.raw_pml4t->entries[pml4t_idx] & PageFlag::Present)) {
                self.raw_pml4t->entries[pml4t_idx] = pmm::allocate_frame() | PageFlag::ReadWriteUser;
                memset(
                        reinterpret_cast<u64 *>((self.raw_pml4t->entries[pml4t_idx] & PHYS_ADDR_MASK) + hhdm_offset),
                        0,
                        PAGE_SIZE
                );
        }

        PageTable *pdpt = reinterpret_cast<PageTable *>((self.raw_pml4t->entries[pml4t_idx] & PHYS_ADDR_MASK) + hhdm_offset);
        if (not (pdpt->entries[pdpt_idx] & PageFlag::Present)) {
                pdpt->entries[pdpt_idx] = pmm::allocate_frame() | PageFlag::ReadWriteUser;
                memset(
                        reinterpret_cast<u64 *>((pdpt->entries[pdpt_idx] & PHYS_ADDR_MASK) + hhdm_offset),
                        0,
                        PAGE_SIZE
                );
        }

        PageTable *pdt = reinterpret_cast<PageTable *>((pdpt->entries[pdpt_idx] & PHYS_ADDR_MASK) + hhdm_offset);
        if (not (pdt->entries[pdt_idx] & PageFlag::Present)) {
                pdt->entries[pdt_idx] = pmm::allocate_frame() | PageFlag::ReadWriteUser;
                memset(
                        reinterpret_cast<u64 *>((pdt->entries[pdt_idx] & PHYS_ADDR_MASK) + hhdm_offset),
                        0,
                        PAGE_SIZE
                );
        }

        PageTable *pt = reinterpret_cast<PageTable *>((pdt->entries[pdt_idx] & PHYS_ADDR_MASK) + hhdm_offset);
        if (not (pt->entries[pt_idx] & PageFlag::Present))
                pt->entries[pt_idx] = paddr | flags;
}

void PML4T::unmap_page(this PML4T &self, uptr vaddr)
{
        uptr hhdm_offset = get_kernel_hhdm_offset();

        u64 pml4t_idx   = (vaddr >> 39) & 0x1ff;
        u64 pdpt_idx    = (vaddr >> 30) & 0x1ff;
        u64 pdt_idx     = (vaddr >> 21) & 0x1ff;
        u64 pt_idx      = (vaddr >> 12) & 0x1ff;

        if (not (self.raw_pml4t->entries[pml4t_idx] & PageFlag::Present))
                return;

        PageTable *pdpt = reinterpret_cast<PageTable *>((self.raw_pml4t->entries[pml4t_idx] & PHYS_ADDR_MASK) + hhdm_offset);
        if (not (pdpt->entries[pdpt_idx] & PageFlag::Present))
                return;

        PageTable *pdt = reinterpret_cast<PageTable *>((pdpt->entries[pdpt_idx] & PHYS_ADDR_MASK) + hhdm_offset);
        if (not (pdt->entries[pdt_idx] & PageFlag::Present))
                return;

        PageTable *pt = reinterpret_cast<PageTable *>((pdt->entries[pdt_idx] & PHYS_ADDR_MASK) + hhdm_offset);
        if (not (pt->entries[pt_idx] & PageFlag::Present))
                return;

        pt->entries[pt_idx] = 0;
        cpu::invlpg(vaddr);

        // A table must have no mapped entries to be deleted

        for (u16 i = 0; i < PAGE_TABLE_ENTRIES; i++) {
                if (pt->entries[i] & PageFlag::Present)
                        return;
        }
        pmm::free_frame(pdt->entries[pdt_idx] & PHYS_ADDR_MASK);
        pdt->entries[pdt_idx] = 0;

        for (u16 i = 0; i < PAGE_TABLE_ENTRIES; i++) {
                if (pdt->entries[i] & PageFlag::Present)
                        return;
        }
        pmm::free_frame(pdpt->entries[pdpt_idx] & PHYS_ADDR_MASK);
        pdpt->entries[pdpt_idx] = 0;

        for (u16 i = 0; i < PAGE_TABLE_ENTRIES; i++) {
                if (pdpt->entries[i] & PageFlag::Present)
                        return;
        }
        pmm::free_frame(self.raw_pml4t->entries[pml4t_idx] & PHYS_ADDR_MASK);
        self.raw_pml4t->entries[pml4t_idx] = 0;
}

bool PML4T::is_mapped(this const PML4T &self, uptr vaddr)
{
        uptr hhdm_offset = get_kernel_hhdm_offset();

        u64 pml4t_idx   = (vaddr >> 39) & 0x1ff;
        u64 pdpt_idx    = (vaddr >> 30) & 0x1ff;
        u64 pdt_idx     = (vaddr >> 21) & 0x1ff;
        u64 pt_idx      = (vaddr >> 12) & 0x1ff;

        if (not (self.raw_pml4t->entries[pml4t_idx] & PageFlag::Present))
                return false;

        PageTable *pdpt = reinterpret_cast<PageTable *>((self.raw_pml4t->entries[pml4t_idx] & PHYS_ADDR_MASK) + hhdm_offset);
        if (not (pdpt->entries[pdpt_idx] & PageFlag::Present))
                return false;

        PageTable *pdt = reinterpret_cast<PageTable *>((pdpt->entries[pdpt_idx] & PHYS_ADDR_MASK) + hhdm_offset);
        if (not (pdt->entries[pdt_idx] & PageFlag::Present))
                return false;

        PageTable *pt = reinterpret_cast<PageTable *>((pdt->entries[pdt_idx] & PHYS_ADDR_MASK) + hhdm_offset);
        return (pt->entries[pt_idx] & PageFlag::Present) != 0;
}

PageTable *PML4T::raw(this const PML4T &self)
{
        return self.raw_pml4t;
}

} /* namespace kernel::mem */