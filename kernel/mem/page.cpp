#include <kernel.hpp>
#include <mem/page.hpp>
#include <mem/pmm.hpp>
#include <lib/typing.hpp>
#include <lib/memory.hpp>

using kernel::lib::u64, kernel::lib::uptr, kernel::lib::usize;
using kernel::lib::memset, kernel::lib::memcpy;

namespace kernel::mem {

uptr pt_deep_copy(PageTable *src, int level)
{
        uptr hhdm_offset = get_kernel_hhdm_offset();

        uptr frame = pmm::allocate_frame();
        PageTable *new_pt = reinterpret_cast<PageTable *>(frame + hhdm_offset);
        memset(new_pt, 0, PAGE_SIZE);

        for (usize i = 0; i < PAGE_TABLE_ENTRIES; i++) {
                u64 entry = src->entries[i];
                if (!(entry & PageFlag::Present))
                        continue;

                u64 flags = entry & ~PHYS_ADDR_MASK;
                uptr child_paddr = entry & PHYS_ADDR_MASK;

                if (level == 1) {
                        uptr frame = pmm::allocate_frame();
                        void *src_page = reinterpret_cast<void *>(child_paddr + hhdm_offset);
                        void *dest_page = reinterpret_cast<void *>(frame + hhdm_offset);
                        memcpy(dest_page, src_page, PAGE_SIZE);
                        new_pt->entries[i] = frame | flags;
                } else {
                        PageTable *child_pt = reinterpret_cast<PageTable *>(child_paddr + hhdm_offset);
                        uptr child_paddr = pt_deep_copy(child_pt, level - 1);
                        new_pt->entries[i] = child_paddr | flags;
                }
        }

        return frame;
}

} /* namespace kernel::mem */