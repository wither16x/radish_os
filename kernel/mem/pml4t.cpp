#include "boot/requests.hpp"
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
	PageTable &PML4T::getOrCreateTable(PageTable &parent, Lib::usize idx)
	{
		Lib::uptr hhdm_offset = kcontext().bootloader.request<Boot::HhdmRequest>().offset;

                if (not parent.isEntryPresent(idx)) {
                        parent.at(idx) = Pmm::allocateFrame() | PageFlag::ReadWriteUser;
                        Lib::memset(
                                reinterpret_cast<Lib::u64 *>((parent.at(idx) & PHYS_ADDR_MASK) + hhdm_offset),
                                0,
                                PAGE_SIZE
                        );
                }

		return *reinterpret_cast<PageTable *>((parent.at(idx) & PHYS_ADDR_MASK) + hhdm_offset);
	}

        void PML4T::init(this PML4T &self)
        {
                Lib::uptr hhdm_offset = kcontext().bootloader.request<Boot::HhdmRequest>().offset;

                Lib::uptr frame = Pmm::allocateFrame();
                self.raw_pml4t = reinterpret_cast<PageTable *>(frame + hhdm_offset);
                Lib::memset(self.raw_pml4t, 0, PAGE_SIZE);
        }

        void PML4T::init(this PML4T &self, const PML4T &parent)
        {
                Lib::uptr hhdm_offset = kcontext().bootloader.request<Boot::HhdmRequest>().offset;

                self.init();

                for (Lib::usize i = PAGE_TABLE_ENTRIES / 2; i < PAGE_TABLE_ENTRIES; i++)
                        self.raw_pml4t->at(i) = parent.raw()->at(i);

                for (Lib::usize i = 0; i < PAGE_TABLE_ENTRIES / 2; i++) {
			if (not parent.raw()->isEntryPresent(i))
				continue;

                        Lib::u64 flags = parent.raw()->at(i) & ~PHYS_ADDR_MASK;
                        Lib::uptr pdpt_phys = parent.raw()->at(i) & PHYS_ADDR_MASK;
                        auto &pdpt = *reinterpret_cast<PageTable *>(pdpt_phys + hhdm_offset);

                        Lib::uptr new_pdpt_phys = ptDeepCopy(pdpt, 3);
                        self.raw_pml4t->at(i) = new_pdpt_phys | flags;
                }
        }

        void PML4T::destroy(this PML4T &self)
        {
                Lib::uptr hhdm_offset = kcontext().bootloader.request<Boot::HhdmRequest>().offset;

                for (Lib::usize pml4t_idx = 0; pml4t_idx < PAGE_TABLE_ENTRIES / 2; pml4t_idx++) {
			if (not self.raw_pml4t->isEntryPresent(pml4t_idx))
				continue;

                        if (pml4t_idx == Mem::Heap::HEAP_PML4T_IDX / 2)
                                continue;

                        auto &pdpt = *reinterpret_cast<PageTable *>((self.raw_pml4t->at(pml4t_idx) & PHYS_ADDR_MASK) + hhdm_offset);
                        for (Lib::u64 pdpt_idx = 0; pdpt_idx < PAGE_TABLE_ENTRIES; pdpt_idx++) {
                                if (not pdpt.isEntryPresent(pdpt_idx))
                                        continue;

                                auto &pdt = *reinterpret_cast<PageTable *>((pdpt.at(pdpt_idx) & PHYS_ADDR_MASK) + hhdm_offset);
                                for (Lib::u16 pdt_idx = 0; pdt_idx < PAGE_TABLE_ENTRIES; pdt_idx++) {
                                        if (not pdt.isEntryPresent(pdt_idx))
                                                continue;

                                        auto &pt = *reinterpret_cast<PageTable *>((pdt.at(pdt_idx) & PHYS_ADDR_MASK) + hhdm_offset);
                                        for (Lib::u64 pt_idx = 0; pt_idx < PAGE_TABLE_ENTRIES; pt_idx++) {
                                                if (not pt.isEntryPresent(pt_idx))
                                                        continue;

                                                Pmm::freeFrame(pt.at(pt_idx) & PHYS_ADDR_MASK);
                                        }
                                        Pmm::freeFrame(reinterpret_cast<Lib::u64>(&pt) - hhdm_offset);
                                }

                                Pmm::freeFrame(reinterpret_cast<Lib::u64>(&pdt) - hhdm_offset);
                        }

                        Pmm::freeFrame(reinterpret_cast<Lib::u64>(&pdpt) - hhdm_offset);
                }

                Pmm::freeFrame(reinterpret_cast<Lib::u64>(self.raw_pml4t) - hhdm_offset);
        }

        void PML4T::load(this const PML4T &self)
        {
                Lib::uptr hhdm_offset = kcontext().bootloader.request<Boot::HhdmRequest>().offset;

                __asm__ volatile ("movq %0, %%cr3" :: "r"(reinterpret_cast<Lib::u64>(self.raw_pml4t) - hhdm_offset));
        }

        void PML4T::mapPage(this PML4T &self, Lib::uptr vaddr, Lib::uptr paddr, Lib::u64 flags)
        {
                Lib::usize pml4t_idx = (vaddr >> 39) & 0x1ff;
                Lib::usize pdpt_idx  = (vaddr >> 30) & 0x1ff;
                Lib::usize pdt_idx   = (vaddr >> 21) & 0x1ff;
                Lib::usize pt_idx    = (vaddr >> 12) & 0x1ff;

		auto &pdpt = self.getOrCreateTable(*self.raw_pml4t, pml4t_idx);
		auto &pdt = self.getOrCreateTable(pdpt, pdpt_idx);
		auto &pt = self.getOrCreateTable(pdt, pdt_idx);

		if (not pt.isEntryPresent(pt_idx))
			pt.at(pt_idx) = paddr | flags;
        }

        void PML4T::unmapPage(this PML4T &self, Lib::uptr vaddr)
        {
                Lib::uptr hhdm_offset = kcontext().bootloader.request<Boot::HhdmRequest>().offset;

                Lib::u64 pml4t_idx   = (vaddr >> 39) & 0x1ff;
                Lib::u64 pdpt_idx    = (vaddr >> 30) & 0x1ff;
                Lib::u64 pdt_idx     = (vaddr >> 21) & 0x1ff;
                Lib::u64 pt_idx      = (vaddr >> 12) & 0x1ff;

                if (not self.raw_pml4t->isEntryPresent(pml4t_idx))
                        return;

                auto &pdpt = *reinterpret_cast<PageTable *>((self.raw_pml4t->at(pml4t_idx) & PHYS_ADDR_MASK) + hhdm_offset);
                if (not pdpt.isEntryPresent(pdpt_idx))
                        return;

                auto &pdt = *reinterpret_cast<PageTable *>((pdpt.at(pdpt_idx) & PHYS_ADDR_MASK) + hhdm_offset);
                if (not pdt.isEntryPresent(pdt_idx))
                        return;

                auto &pt = *reinterpret_cast<PageTable *>((pdt.at(pdt_idx) & PHYS_ADDR_MASK) + hhdm_offset);
                if (not pt.isEntryPresent(pt_idx))
                        return;

                pt.at(pt_idx) = 0;
                Cpu::invlpg(vaddr);

                // A table must have no mapped entries to be deleted

                for (Lib::u16 i = 0; i < PAGE_TABLE_ENTRIES; i++) {
                        if (pt.isEntryPresent(i))
                                return;
                }
                Pmm::freeFrame(pdt.at(pdt_idx) & PHYS_ADDR_MASK);
                pdt.at(pdt_idx) = 0;

                for (Lib::u16 i = 0; i < PAGE_TABLE_ENTRIES; i++) {
                        if (pdt.isEntryPresent(i))
                                return;
                }
                Pmm::freeFrame(pdpt.at(pdpt_idx) & PHYS_ADDR_MASK);
                pdpt.at(pdpt_idx) = 0;

                for (Lib::u16 i = 0; i < PAGE_TABLE_ENTRIES; i++) {
                        if (pdpt.isEntryPresent(i))
                                return;
                }
                Pmm::freeFrame(self.raw_pml4t->at(pml4t_idx) & PHYS_ADDR_MASK);
                self.raw_pml4t->at(pml4t_idx) = 0;
        }

        Lib::uptr PML4T::virtToPhys(this const PML4T &self, Lib::uptr vaddr)
        {
                Lib::uptr hhdm_offset = kcontext().bootloader.request<Boot::HhdmRequest>().offset;

                Lib::u64 pml4t_idx   = (vaddr >> 39) & 0x1ff;
                Lib::u64 pdpt_idx    = (vaddr >> 30) & 0x1ff;
                Lib::u64 pdt_idx     = (vaddr >> 21) & 0x1ff;
                Lib::u64 pt_idx      = (vaddr >> 12) & 0x1ff;

                if (not self.raw_pml4t->isEntryPresent(pml4t_idx))
                        return 0;

                auto &pdpt = *reinterpret_cast<PageTable *>((self.raw_pml4t->at(pml4t_idx) & PHYS_ADDR_MASK) + hhdm_offset);
                if (not pdpt.isEntryPresent(pdpt_idx))
                        return 0;

                auto &pdt = *reinterpret_cast<PageTable *>((pdpt.at(pdpt_idx) & PHYS_ADDR_MASK) + hhdm_offset);
                if (not pdt.isEntryPresent(pdt_idx))
                        return 0;

                auto &pt = *reinterpret_cast<PageTable *>((pdt.at(pdt_idx) & PHYS_ADDR_MASK) + hhdm_offset);
                return (pt.entries[pt_idx] & PHYS_ADDR_MASK) + (vaddr & 0xfff);
        }

        bool PML4T::isMapped(this const PML4T &self, Lib::uptr vaddr)
        {
                Lib::uptr hhdm_offset = kcontext().bootloader.request<Boot::HhdmRequest>().offset;

                Lib::u64 pml4t_idx   = (vaddr >> 39) & 0x1ff;
                Lib::u64 pdpt_idx    = (vaddr >> 30) & 0x1ff;
                Lib::u64 pdt_idx     = (vaddr >> 21) & 0x1ff;
                Lib::u64 pt_idx      = (vaddr >> 12) & 0x1ff;

                if (not self.raw_pml4t->isEntryPresent(pml4t_idx))
                        return false;

                auto &pdpt = *reinterpret_cast<PageTable *>((self.raw_pml4t->at(pml4t_idx) & PHYS_ADDR_MASK) + hhdm_offset);
                if (not pdpt.isEntryPresent(pdpt_idx))
                        return false;

                auto &pdt = *reinterpret_cast<PageTable *>((pdpt.at(pdpt_idx) & PHYS_ADDR_MASK) + hhdm_offset);
                if (not pdt.isEntryPresent(pdt_idx))
                        return false;

                auto &pt = *reinterpret_cast<PageTable *>((pdt.at(pdt_idx) & PHYS_ADDR_MASK) + hhdm_offset);
                return pt.isEntryPresent(pt_idx);
        }

        PageTable *PML4T::raw(this const PML4T &self)
        {
                return self.raw_pml4t;
        }
} // namespace Kiwi::Mem