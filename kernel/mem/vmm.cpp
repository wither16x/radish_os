#include <kernel.hpp>
#include <boot/bootinfo.hpp>
#include <cpu/assembly.hpp>
#include <lib/logging.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <mem/heap.hpp>
#include <mem/pml4t.hpp>
#include <mem/page.hpp>

using kernel::lib::u16, kernel::lib::u64, kernel::lib::usize, kernel::lib::uptr;
using kernel::lib::log::logger;

extern "C" u64 _lds_kernel_start[];
extern "C" u64 _lds_kernel_end[];

namespace kernel::mem::vmm {

namespace {

u64 hhdm = 0;
u64 executable_phys = 0;
u64 executable_virt = 0;
boot::BootInfo::MemmapInfo memmap_info;

/// Map the kernel in memory.
void map_kernel(PML4T *pml4t)
{
        u64 kstart      = reinterpret_cast<u64>(*&_lds_kernel_start);
        u64 kend        = reinterpret_cast<u64>(*&_lds_kernel_end);
        u64 ksize       = page_div_up(kend - kstart);
        u64 phys_addr   = executable_phys;
        u64 virt_addr   = executable_virt;

        for (u64 i = 0; i < ksize; i++) {
                pml4t->map_page(virt_addr, phys_addr, PageFlag::ReadWriteUser);
                phys_addr += PAGE_SIZE;
                virt_addr += PAGE_SIZE;
        }

        u64 stack_pages = KERNEL_STACK_SIZE / PAGE_SIZE;
        virt_addr = KERNEL_STACK_BOTTOM;
        
        for (usize i = 0; i < stack_pages; i++) {
                pml4t->map_page(virt_addr, pmm::allocate_frame(), PageFlag::ReadWrite | PageFlag::NoExec);
                virt_addr += PAGE_SIZE;
        }
}

/// Map the working memory in higher half.
void map_hhdm(PML4T *pml4t)
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
                                pml4t->map_page(virt_addr, phys_addr, PageFlag::ReadWriteUser);
                                virt_addr += PAGE_SIZE;
                                phys_addr += PAGE_SIZE;
                        }
                }
	}
}

} /* anonymous namespace */

// --------------------------------------------------
PML4T init(u64 hhdm_base,
        boot::BootInfo::ExecutableInfo &exec_info,
        boot::BootInfo::MemmapInfo &_memmap_info
)
{
        hhdm = hhdm_base;
        executable_phys = exec_info.physical_base;
        executable_virt = exec_info.virtual_base;
        memmap_info = _memmap_info;

        PML4T pml4t;
        pml4t.init();

        map_kernel(&pml4t);
        map_hhdm(&pml4t);

        logger.ok("initialized vmm");

        return pml4t;
}
// --------------------------------------------------

} /* namespace kernel::mem::vmm */