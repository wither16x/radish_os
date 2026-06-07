#include <boot/bootinfo.hpp>
#include <cpu/assembly.hpp>
#include <lib/logging.hpp>
#include <lib/memory.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>

using kernel::lib::u16, kernel::lib::u64, kernel::lib::usize, kernel::lib::uptr;
using kernel::lib::log::logger;
using kernel::lib::memset;

extern "C" u64 _lds_kernel_start[];
extern "C" u64 _lds_kernel_end[];

namespace kernel::mem {

namespace {

constexpr usize PhysicalAddressMask     = ((1ull << 52) - 1) & ~0xfffull;
constexpr u16   PageTableEntries        = 512;

}

void VMM::init(this VMM &self,
        lib::u64 hhdm,
        boot::BootInfo::ExecutableInfo &executable_info,
        boot::BootInfo::MemmapInfo &memmap_info
)
{
        self.hhdm = hhdm;
        self.executable_info = executable_info;
        self.memmap_info = memmap_info;

        self.pml4t = reinterpret_cast<u64 *>(pmm.allocate_frame() + self.hhdm);
        memset(self.pml4t, 0, self.PageBytes);

        self.map_kernel();
        self.map_hhdm();

        logger.ok("initialized vmm");
}

void VMM::load(this const VMM &self)
{
        __asm__ volatile ("movq %0, %%cr3" :: "r"(reinterpret_cast<u64>(self.pml4t) - self.hhdm));
}

void VMM::map_page(this VMM &self, lib::uptr virt, lib::uptr phys, lib::u64 flags)
{
        // 0x1 = present
        // 0x03 = rw cpl0
        // (gonna add constants for this later)

        // create intermediate page tables containing informations
        // that tell the CPU where the corresponding frame is

        // split the virtual address to find the its indices in the
        // page tables
        usize pml4t_idx = (virt >> 39) & 0x1ff;
        usize pdpt_idx  = (virt >> 30) & 0x1ff;
        usize pdt_idx   = (virt >> 21) & 0x1ff;
        usize pt_idx    = (virt >> 12) & 0x1ff;

        // now map the page to the given frame
        if (!(self.pml4t[pml4t_idx] & 1)) {
                self.pml4t[pml4t_idx] = pmm.allocate_frame() + 0x03;
                memset(
                        reinterpret_cast<u64 *>((self.pml4t[pml4t_idx] & PhysicalAddressMask) + self.hhdm),
                        0,
                        self.PageBytes
                );
        }

        u64 *pdpt = reinterpret_cast<u64 *>((self.pml4t[pml4t_idx] & PhysicalAddressMask) + self.hhdm);
        if (!(pdpt[pdpt_idx] & 1)) {
                pdpt[pdpt_idx] = pmm.allocate_frame() + 0x03;
                memset(
                        reinterpret_cast<u64 *>((pdpt[pdpt_idx] & PhysicalAddressMask) + self.hhdm),
                        0,
                        self.PageBytes
                );
        }

        u64 *pdt = reinterpret_cast<u64 *>((pdpt[pdpt_idx] & PhysicalAddressMask) + self.hhdm);
        if (!(pdt[pdt_idx] & 1)) {
                pdt[pdt_idx] = pmm.allocate_frame() + 0x03;
                memset(
                        reinterpret_cast<u64 *>((pdt[pdt_idx] & PhysicalAddressMask) + self.hhdm),
                        0,
                        self.PageBytes
                );
        }

        u64 *pt = reinterpret_cast<u64 *>((pdt[pdt_idx] & PhysicalAddressMask) + self.hhdm);
        if (!(pt[pt_idx] & 1))
                pt[pt_idx] = phys | flags;
}

void VMM::unmap_page(this VMM &self, uptr virt)
{
        u64 pml4t_idx   = (virt >> 39) & 0x1ff;
        u64 pdpt_idx    = (virt >> 30) & 0x1ff;
        u64 pdt_idx     = (virt >> 21) & 0x1ff;
        u64 pt_idx      = (virt >> 12) & 0x1ff;

        if (!(self.pml4t[pml4t_idx] & 1))
                return;

        u64 *pdpt = reinterpret_cast<u64 *>((self.pml4t[pml4t_idx] & PhysicalAddressMask) + self.hhdm);
        if (!(pdpt[pdpt_idx] & 1))
                return;

        u64 *pdt = reinterpret_cast<u64 *>((pdpt[pdpt_idx] & PhysicalAddressMask) + self.hhdm);
        if (!(pdt[pdt_idx] & 1))
                return;

        u64 *pt = reinterpret_cast<u64 *>((pdt[pdt_idx] & PhysicalAddressMask) + self.hhdm);
        if (!(pt[pt_idx] & 1))
                return;

        pt[pt_idx] = 0;
        cpu::invlpg(virt);

        // A table must have no mapped entries to be deleted

        for (u16 i = 0; i < PageTableEntries; i++) {
                if (pt[i] & 1)
                        return;
        }
        pmm.free_frame(pdt[pdt_idx] & PhysicalAddressMask);
        pdt[pdt_idx] = 0;

        for (u16 i = 0; i < PageTableEntries; i++) {
                if (pdt[i] & 1)
                        return;
        }
        pmm.free_frame(pdpt[pdpt_idx] & PhysicalAddressMask);
        pdpt[pdpt_idx] = 0;

        for (u16 i = 0; i < PageTableEntries; i++) {
                if (pdpt[i] & 1)
                        return;
        }
        pmm.free_frame(self.pml4t[pml4t_idx] & PhysicalAddressMask);
        self.pml4t[pml4t_idx] = 0;
}

uptr VMM::get_pml4t()
{
        u64 cr3;
        __asm__ ("movq %%cr3, %0" : "=r"(cr3));
        return cr3;
}

void VMM::map_kernel(this VMM &self)
{
        u64 kstart      = reinterpret_cast<u64>(*&_lds_kernel_start);
        u64 kend        = reinterpret_cast<u64>(*&_lds_kernel_end);
        u64 ksize       = self.page_div_up(kend - kstart);
        u64 phys_addr   = self.executable_info.physical_base;
        u64 virt_addr   = self.executable_info.virtual_base;

        for (u64 i = 0; i < ksize; i++) {
                self.map_page(virt_addr, phys_addr, 0x03);
                phys_addr += self.PageBytes;
                virt_addr += self.PageBytes;
        }
}

void VMM::map_hhdm(this VMM &self)
{
	for (u64 index = 0; index < self.memmap_info.entry_count; index++) {
		boot::MemmapEntryType type = self.memmap_info.entries[index].type;

		if (type == boot::MemmapEntryType::ACPINVS ||
		    type == boot::MemmapEntryType::ACPIReclaimable ||
		    type == boot::MemmapEntryType::BootloaderReclaimable ||
		    type == boot::MemmapEntryType::Framebuffer ||
		    type == boot::MemmapEntryType::ExecutableAndModules ||
		    type == boot::MemmapEntryType::Usable
		) {
                        u64 section_size = self.page_div_up(self.memmap_info.entries[index].length);
                        u64 phys_addr = self.memmap_info.entries[index].base;
                        u64 virt_addr = self.memmap_info.entries[index].base + self.hhdm;
                        
                        for (u64 i = 0; i < section_size; i++) {
                                self.map_page(virt_addr, phys_addr, 0x03);
                                virt_addr += self.PageBytes;
                                phys_addr += self.PageBytes;
                        }
                }
	}
}

} /* namespace kernel::mem */