#include <kernel.hpp>
#include <mem/pml4t.hpp>
#include <mem/pmm.hpp>
#include <mem/heap.hpp>
#include <mem/page.hpp>
#include <lib/typing.hpp>
#include <lib/memory.hpp>
#include <cpu/assembly.hpp>

namespace Kiwi::Mem
{
        void PML4T::init(this PML4T &self)
        {
                Lib::uptr hhdm_offset = getKernelHhdmOffset();

                Lib::uptr frame = Pmm::allocateFrame();
                self.raw_pml4t = reinterpret_cast<PageTable *>(frame + hhdm_offset);
                Lib::memset(self.raw_pml4t, 0, PAGE_SIZE);     
        }

        void PML4T::init(this PML4T &self, const PML4T &parent)
        {
                Lib::uptr hhdm_offset = getKernelHhdmOffset();

                self.init();

                for (Lib::usize i = PAGE_TABLE_ENTRIES / 2; i < PAGE_TABLE_ENTRIES; i++)
                        self.raw_pml4t->entries[i] = parent.raw()->entries[i];

                for (Lib::usize i = 0; i < PAGE_TABLE_ENTRIES / 2; i++) {
                        Lib::u64 entry = parent.raw()->entries[i];
                        if (not (entry & PageFlag::Present))
                                continue;

                        Lib::u64 flags = entry & ~PHYS_ADDR_MASK;
                        Lib::uptr pdpt_phys = entry & PHYS_ADDR_MASK;
                        PageTable *pdpt = reinterpret_cast<PageTable *>(pdpt_phys + hhdm_offset);

                        Lib::uptr new_pdpt_phys = ptDeepCopy(pdpt, 3);
                        self.raw_pml4t->entries[i] = new_pdpt_phys | flags;
                }
        }

        void PML4T::destroy(this PML4T &self)
        {
                Lib::uptr hhdm_offset = getKernelHhdmOffset();

                for (Lib::usize pml4t_idx = 0; pml4t_idx < PAGE_TABLE_ENTRIES / 2; pml4t_idx++) {
                        if (not (self.raw_pml4t->entries[pml4t_idx] & PageFlag::Present))
                                continue;

                        if (pml4t_idx == Mem::Heap::HEAP_PML4T_IDX / 2)
                                continue;

                        PageTable *pdpt = reinterpret_cast<PageTable *>((self.raw_pml4t->entries[pml4t_idx] & PHYS_ADDR_MASK) + hhdm_offset);
                        for (Lib::u64 pdpt_idx = 0; pdpt_idx < PAGE_TABLE_ENTRIES; pdpt_idx++) {
                                if (not (pdpt->entries[pdpt_idx] & PageFlag::Present))
                                        continue;

                                PageTable *pdt = reinterpret_cast<PageTable *>((pdpt->entries[pdpt_idx] & PHYS_ADDR_MASK) + hhdm_offset);
                                for (Lib::u16 pdt_idx = 0; pdt_idx < PAGE_TABLE_ENTRIES; pdt_idx++) {
                                        if (not (pdt->entries[pdt_idx] & PageFlag::Present))
                                                continue;

                                        PageTable *pt = reinterpret_cast<PageTable *>((pdt->entries[pdt_idx] & PHYS_ADDR_MASK) + hhdm_offset);
                                        for (Lib::u64 pt_idx = 0; pt_idx < PAGE_TABLE_ENTRIES; pt_idx++) {
                                                if (not (pt->entries[pt_idx] & PageFlag::Present))
                                                        continue;

                                                Pmm::freeFrame(pt->entries[pt_idx] & PHYS_ADDR_MASK);
                                        }
                                        Pmm::freeFrame(reinterpret_cast<Lib::u64>(pt->entries) - hhdm_offset);
                                }

                                Pmm::freeFrame(reinterpret_cast<Lib::u64>(pdt) - hhdm_offset);
                        }

                        Pmm::freeFrame(reinterpret_cast<Lib::u64>(pdpt) - hhdm_offset);
                }

                Pmm::freeFrame(reinterpret_cast<Lib::u64>(self.raw_pml4t) - hhdm_offset);
        }

        void PML4T::load(this const PML4T &self)
        {
                Lib::uptr hhdm_offset = getKernelHhdmOffset();

                __asm__ volatile ("movq %0, %%cr3" :: "r"(reinterpret_cast<Lib::u64>(self.raw_pml4t) - hhdm_offset));
        }

        void PML4T::mapPage(this PML4T &self, Lib::uptr vaddr, Lib::uptr paddr, Lib::u64 flags)
        {
                Lib::uptr hhdm_offset = getKernelHhdmOffset();

                // create intermediate page tables containing informations
                // that tell the CPU where the corresponding frame is

                // split the virtual address to find the its indices in the
                // page tables
                Lib::usize pml4t_idx = (vaddr >> 39) & 0x1ff;
                Lib::usize pdpt_idx  = (vaddr >> 30) & 0x1ff;
                Lib::usize pdt_idx   = (vaddr >> 21) & 0x1ff;
                Lib::usize pt_idx    = (vaddr >> 12) & 0x1ff;

                // now map the page to the given frame
                if (not (self.raw_pml4t->entries[pml4t_idx] & PageFlag::Present)) {
                        self.raw_pml4t->entries[pml4t_idx] = Pmm::allocateFrame() | PageFlag::ReadWriteUser;
                        Lib::memset(
                                reinterpret_cast<Lib::u64 *>((self.raw_pml4t->entries[pml4t_idx] & PHYS_ADDR_MASK) + hhdm_offset),
                                0,
                                PAGE_SIZE
                        );
                }

                PageTable *pdpt = reinterpret_cast<PageTable *>((self.raw_pml4t->entries[pml4t_idx] & PHYS_ADDR_MASK) + hhdm_offset);
                if (not (pdpt->entries[pdpt_idx] & PageFlag::Present)) {
                        pdpt->entries[pdpt_idx] = Pmm::allocateFrame() | PageFlag::ReadWriteUser;
                        Lib::memset(
                                reinterpret_cast<Lib::u64 *>((pdpt->entries[pdpt_idx] & PHYS_ADDR_MASK) + hhdm_offset),
                                0,
                                PAGE_SIZE
                        );
                }

                PageTable *pdt = reinterpret_cast<PageTable *>((pdpt->entries[pdpt_idx] & PHYS_ADDR_MASK) + hhdm_offset);
                if (not (pdt->entries[pdt_idx] & PageFlag::Present)) {
                        pdt->entries[pdt_idx] = Pmm::allocateFrame() | PageFlag::ReadWriteUser;
                        Lib::memset(
                                reinterpret_cast<Lib::u64 *>((pdt->entries[pdt_idx] & PHYS_ADDR_MASK) + hhdm_offset),
                                0,
                                PAGE_SIZE
                        );
                }

                PageTable *pt = reinterpret_cast<PageTable *>((pdt->entries[pdt_idx] & PHYS_ADDR_MASK) + hhdm_offset);
                if (not (pt->entries[pt_idx] & PageFlag::Present))
                        pt->entries[pt_idx] = paddr | flags;
        }

        void PML4T::unmapPage(this PML4T &self, Lib::uptr vaddr)
        {
                Lib::uptr hhdm_offset = getKernelHhdmOffset();

                Lib::u64 pml4t_idx   = (vaddr >> 39) & 0x1ff;
                Lib::u64 pdpt_idx    = (vaddr >> 30) & 0x1ff;
                Lib::u64 pdt_idx     = (vaddr >> 21) & 0x1ff;
                Lib::u64 pt_idx      = (vaddr >> 12) & 0x1ff;

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
                Cpu::invlpg(vaddr);

                // A table must have no mapped entries to be deleted

                for (Lib::u16 i = 0; i < PAGE_TABLE_ENTRIES; i++) {
                        if (pt->entries[i] & PageFlag::Present)
                                return;
                }
                Pmm::freeFrame(pdt->entries[pdt_idx] & PHYS_ADDR_MASK);
                pdt->entries[pdt_idx] = 0;

                for (Lib::u16 i = 0; i < PAGE_TABLE_ENTRIES; i++) {
                        if (pdt->entries[i] & PageFlag::Present)
                                return;
                }
                Pmm::freeFrame(pdpt->entries[pdpt_idx] & PHYS_ADDR_MASK);
                pdpt->entries[pdpt_idx] = 0;

                for (Lib::u16 i = 0; i < PAGE_TABLE_ENTRIES; i++) {
                        if (pdpt->entries[i] & PageFlag::Present)
                                return;
                }
                Pmm::freeFrame(self.raw_pml4t->entries[pml4t_idx] & PHYS_ADDR_MASK);
                self.raw_pml4t->entries[pml4t_idx] = 0;
        }

        Lib::uptr PML4T::virtToPhys(this const PML4T &self, Lib::uptr vaddr)
        {
                Lib::uptr hhdm_offset = getKernelHhdmOffset();

                Lib::u64 pml4t_idx   = (vaddr >> 39) & 0x1ff;
                Lib::u64 pdpt_idx    = (vaddr >> 30) & 0x1ff;
                Lib::u64 pdt_idx     = (vaddr >> 21) & 0x1ff;
                Lib::u64 pt_idx      = (vaddr >> 12) & 0x1ff;

                if (not (self.raw_pml4t->entries[pml4t_idx] & PageFlag::Present))
                        return 0;

                PageTable *pdpt = reinterpret_cast<PageTable *>((self.raw_pml4t->entries[pml4t_idx] & PHYS_ADDR_MASK) + hhdm_offset);
                if (not (pdpt->entries[pdpt_idx] & PageFlag::Present))
                        return 0;

                PageTable *pdt = reinterpret_cast<PageTable *>((pdpt->entries[pdpt_idx] & PHYS_ADDR_MASK) + hhdm_offset);
                if (not (pdt->entries[pdt_idx] & PageFlag::Present))
                        return 0;

                PageTable *pt = reinterpret_cast<PageTable *>((pdt->entries[pdt_idx] & PHYS_ADDR_MASK) + hhdm_offset);
                return (pt->entries[pt_idx] & PHYS_ADDR_MASK) + (vaddr & 0xfff);
        }

        bool PML4T::isMapped(this const PML4T &self, Lib::uptr vaddr)
        {
                Lib::uptr hhdm_offset = getKernelHhdmOffset();

                Lib::u64 pml4t_idx   = (vaddr >> 39) & 0x1ff;
                Lib::u64 pdpt_idx    = (vaddr >> 30) & 0x1ff;
                Lib::u64 pdt_idx     = (vaddr >> 21) & 0x1ff;
                Lib::u64 pt_idx      = (vaddr >> 12) & 0x1ff;

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
} // namespace Kiwi::Mem