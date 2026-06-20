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

namespace kernel::mem::vmm {

namespace {

constexpr usize PHYS_ADDR_MASK     = ((1ull << 52) - 1) & ~0xfffull;
constexpr u16   PT_ENTRIES         = 512;

u64 *pml4t = nullptr;
u64 hhdm = 0;
u64 executable_phys = 0;
u64 executable_virt = 0;
boot::BootInfo::MemmapInfo memmap_info;

void map_kernel()
{
        u64 kstart      = reinterpret_cast<u64>(*&_lds_kernel_start);
        u64 kend        = reinterpret_cast<u64>(*&_lds_kernel_end);
        u64 ksize       = page_div_up(kend - kstart);
        u64 phys_addr   = executable_phys;
        u64 virt_addr   = executable_virt;

        for (u64 i = 0; i < ksize; i++) {
                map_page(virt_addr, phys_addr, 0x03);
                phys_addr += PAGE_BYTES;
                virt_addr += PAGE_BYTES;
        }
}

void map_hhdm()
{
	for (u64 index = 0; index < memmap_info.entry_count; index++) {
		boot::MemmapEntryType type = memmap_info.entries[index].type;

		if (type == boot::MemmapEntryType::ACPINVS ||
		    type == boot::MemmapEntryType::ACPIReclaimable ||
		    type == boot::MemmapEntryType::BootloaderReclaimable ||
		    type == boot::MemmapEntryType::Framebuffer ||
		    type == boot::MemmapEntryType::ExecutableAndModules ||
		    type == boot::MemmapEntryType::Usable
		) {
                        u64 section_size = page_div_up(memmap_info.entries[index].length);
                        u64 phys_addr = memmap_info.entries[index].base;
                        u64 virt_addr = memmap_info.entries[index].base + hhdm;
                        
                        for (u64 i = 0; i < section_size; i++) {
                                map_page(virt_addr, phys_addr, 0x03);
                                virt_addr += PAGE_BYTES;
                                phys_addr += PAGE_BYTES;
                        }
                }
	}
}

} /* anonymous namespace */

void init(lib::u64 hhdm_base,
        boot::BootInfo::ExecutableInfo &exec_info,
        boot::BootInfo::MemmapInfo &_memmap_info
)
{
        hhdm = hhdm_base;
        executable_phys = exec_info.physical_base;
        executable_virt = exec_info.virtual_base;
        memmap_info = _memmap_info;

        pml4t = reinterpret_cast<u64 *>(pmm::allocate_frame() + hhdm);
        memset(pml4t, 0, PAGE_BYTES);

        map_kernel();
        map_hhdm();

        logger.ok("initialized vmm");
}

void load()
{
        __asm__ volatile ("movq %0, %%cr3" :: "r"(reinterpret_cast<u64>(pml4t) - hhdm));
}

void map_page(lib::uptr virt, lib::uptr phys, lib::u64 flags)
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
        if (!(pml4t[pml4t_idx] & 1)) {
                pml4t[pml4t_idx] = pmm::allocate_frame() + 0x03;
                memset(
                        reinterpret_cast<u64 *>((pml4t[pml4t_idx] & PHYS_ADDR_MASK) + hhdm),
                        0,
                        PAGE_BYTES
                );
        }

        u64 *pdpt = reinterpret_cast<u64 *>((pml4t[pml4t_idx] & PHYS_ADDR_MASK) + hhdm);
        if (!(pdpt[pdpt_idx] & 1)) {
                pdpt[pdpt_idx] = pmm::allocate_frame() + 0x03;
                memset(
                        reinterpret_cast<u64 *>((pdpt[pdpt_idx] & PHYS_ADDR_MASK) + hhdm),
                        0,
                        PAGE_BYTES
                );
        }

        u64 *pdt = reinterpret_cast<u64 *>((pdpt[pdpt_idx] & PHYS_ADDR_MASK) + hhdm);
        if (!(pdt[pdt_idx] & 1)) {
                pdt[pdt_idx] = pmm::allocate_frame() + 0x03;
                memset(
                        reinterpret_cast<u64 *>((pdt[pdt_idx] & PHYS_ADDR_MASK) + hhdm),
                        0,
                        PAGE_BYTES
                );
        }

        u64 *pt = reinterpret_cast<u64 *>((pdt[pdt_idx] & PHYS_ADDR_MASK) + hhdm);
        if (!(pt[pt_idx] & 1))
                pt[pt_idx] = phys | flags;
}

void unmap_page(uptr virt)
{
        u64 pml4t_idx   = (virt >> 39) & 0x1ff;
        u64 pdpt_idx    = (virt >> 30) & 0x1ff;
        u64 pdt_idx     = (virt >> 21) & 0x1ff;
        u64 pt_idx      = (virt >> 12) & 0x1ff;

        if (!(pml4t[pml4t_idx] & 1))
                return;

        u64 *pdpt = reinterpret_cast<u64 *>((pml4t[pml4t_idx] & PHYS_ADDR_MASK) + hhdm);
        if (!(pdpt[pdpt_idx] & 1))
                return;

        u64 *pdt = reinterpret_cast<u64 *>((pdpt[pdpt_idx] & PHYS_ADDR_MASK) + hhdm);
        if (!(pdt[pdt_idx] & 1))
                return;

        u64 *pt = reinterpret_cast<u64 *>((pdt[pdt_idx] & PHYS_ADDR_MASK) + hhdm);
        if (!(pt[pt_idx] & 1))
                return;

        pt[pt_idx] = 0;
        cpu::invlpg(virt);

        // A table must have no mapped entries to be deleted

        for (u16 i = 0; i < PT_ENTRIES; i++) {
                if (pt[i] & 1)
                        return;
        }
        pmm::free_frame(pdt[pdt_idx] & PHYS_ADDR_MASK);
        pdt[pdt_idx] = 0;

        for (u16 i = 0; i < PT_ENTRIES; i++) {
                if (pdt[i] & 1)
                        return;
        }
        pmm::free_frame(pdpt[pdpt_idx] & PHYS_ADDR_MASK);
        pdpt[pdpt_idx] = 0;

        for (u16 i = 0; i < PT_ENTRIES; i++) {
                if (pdpt[i] & 1)
                        return;
        }
        pmm::free_frame(pml4t[pml4t_idx] & PHYS_ADDR_MASK);
        pml4t[pml4t_idx] = 0;
}

} /* namespace kernel::mem::vmm */