#include <kernel.hpp>
#include <boot/bootinfo.hpp>
#include <cpu/assembly.hpp>
#include <lib/logging.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <mem/heap.hpp>
#include <mem/pml4t.hpp>
#include <mem/page.hpp>

extern "C" Kiwi::Lib::u64 _lds_kernel_start[];
extern "C" Kiwi::Lib::u64 _lds_kernel_end[];

namespace Kiwi::Mem::Vmm
{
        namespace
        {
                Lib::u64 hhdm = 0;
                Lib::u64 executable_phys = 0;
                Lib::u64 executable_virt = 0;
                Boot::BootInfo::MemmapInfo memmap_info;

                /// Map the kernel in memory.
                void map_kernel(PML4T *pml4t)
                {
                        Lib::u64 kstart      = reinterpret_cast<Lib::u64>(*&_lds_kernel_start);
                        Lib::u64 kend        = reinterpret_cast<Lib::u64>(*&_lds_kernel_end);
                        Lib::u64 ksize       = pageDivUp(kend - kstart);
                        Lib::u64 phys_addr   = executable_phys;
                        Lib::u64 virt_addr   = executable_virt;

                        for (Lib::u64 i = 0; i < ksize; i++) {
                                pml4t->mapPage(virt_addr, phys_addr, PageFlag::ReadWriteUser);
                                phys_addr += PAGE_SIZE;
                                virt_addr += PAGE_SIZE;
                        }

                        Lib::u64 stack_pages = KERNEL_STACK_SIZE / PAGE_SIZE;
                        virt_addr = KERNEL_STACK_BOTTOM;
                        
                        for (Lib::usize i = 0; i < stack_pages; i++) {
                                pml4t->mapPage(virt_addr, Pmm::allocateFrame(), PageFlag::ReadWrite | PageFlag::NoExec);
                                virt_addr += PAGE_SIZE;
                        }
                }

                /// Map the working memory in higher half.
                void map_hhdm(PML4T *pml4t)
                {
                        for (Lib::u64 index = 0; index < memmap_info.entry_count; index++) {
                                Boot::MemmapEntryType type = memmap_info.entries[index].type;

                                if (type == Boot::MemmapEntryType::AcpiNvs or
                                type == Boot::MemmapEntryType::AcpiReclaimable or
                                type == Boot::MemmapEntryType::BootloaderReclaimable or
                                type == Boot::MemmapEntryType::Framebuffer or
                                type == Boot::MemmapEntryType::ExecutableAndModules or
                                type == Boot::MemmapEntryType::Usable
                                ) {
                                        Lib::u64 section_size = pageDivUp(memmap_info.entries[index].length);
                                        Lib::u64 phys_addr = memmap_info.entries[index].base;
                                        Lib::u64 virt_addr = memmap_info.entries[index].base + hhdm;
                                        
                                        for (Lib::u64 i = 0; i < section_size; i++) {
                                                pml4t->mapPage(virt_addr, phys_addr, PageFlag::ReadWriteUser);
                                                virt_addr += PAGE_SIZE;
                                                phys_addr += PAGE_SIZE;
                                        }
                                }
                        }
                }
        } // anonymous namespace

        PML4T init(Lib::u64 hhdm_base,
                Boot::BootInfo::ExecutableInfo &exec_info,
                Boot::BootInfo::MemmapInfo &_memmap_info
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

                Lib::Log::logger.ok("initialized vmm");

                return pml4t;
        }
} // namespace Kiwi::Mem::Vmm