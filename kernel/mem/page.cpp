#include <kernel.hpp>
#include <mem/page.hpp>
#include <mem/pmm.hpp>
#include <lib/typing.hpp>
#include <lib/memory.hpp>

namespace Kiwi::Mem
{
        Lib::uptr ptDeepCopy(PageTable *src, int level)
        {
                Lib::uptr hhdm_offset = getKernelHhdmOffset();

                Lib::uptr frame = Pmm::allocateFrame();
                PageTable *new_pt = reinterpret_cast<PageTable *>(frame + hhdm_offset);
                Lib::memset(new_pt, 0, PAGE_SIZE);

                for (Lib::usize i = 0; i < PAGE_TABLE_ENTRIES; i++) {
                        Lib::u64 entry = src->entries[i];
                        if (not (entry & PageFlag::Present))
                                continue;

                        Lib::u64 flags = entry & ~PHYS_ADDR_MASK;
                        Lib::uptr child_paddr = entry & PHYS_ADDR_MASK;

                        if (level == 1) {
                                Lib::uptr frame = Pmm::allocateFrame();
                                void *src_page = reinterpret_cast<void *>(child_paddr + hhdm_offset);
                                void *dest_page = reinterpret_cast<void *>(frame + hhdm_offset);
                                Lib::memcpy(dest_page, src_page, PAGE_SIZE);
                                new_pt->entries[i] = frame | flags;
                        } else {
                                PageTable *child_pt = reinterpret_cast<PageTable *>(child_paddr + hhdm_offset);
                                Lib::uptr child_paddr = ptDeepCopy(child_pt, level - 1);
                                new_pt->entries[i] = child_paddr | flags;
                        }
                }

                return frame;
        }
} // namespace Kiwi::Mem