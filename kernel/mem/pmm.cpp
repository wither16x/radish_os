#include <boot/bootinfo.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>
#include <mem/allocators/static_bitmap.hpp>
#include <mem/allocators/dynamic_bitmap.hpp>
#include <mem/pmm.hpp>

using kernel::lib::log::logger;
using kernel::lib::u8,  kernel::lib::usize, kernel::lib::uptr, kernel::lib::u64;

namespace kernel::mem::pmm {

namespace {

constexpr lib::usize FRAME_BYTES  = 0x1000;       // 4 KiB
constexpr lib::usize MAX_MEMORY   = 0x40000000;   // 1 GiB
constexpr lib::usize MAX_FRAMES   = MAX_MEMORY / FRAME_BYTES;

allocators::StaticBitmapAllocator<uptr, MAX_FRAMES> allocator_stage1;

} /* anonymous namespace */

// --------------------------------------------------
void init(boot::BootInfo::MemmapInfo &memmap)
{
        allocator_stage1.get_bitmap().set_all();

        for (usize i = 0; i < memmap.entry_count; i++) {
                if (memmap.entries[i].type == boot::MemmapEntryType::Usable) {
                        boot::MemmapEntry& e = memmap.entries[i];
                        for (uptr addr = e.base; addr < e.base + e.length; addr += FRAME_BYTES)
                                allocator_stage1.get_bitmap().clear(addr / FRAME_BYTES);
                }
        }

        logger.ok("initialized pmm stage 1");
}
// --------------------------------------------------

// --------------------------------------------------
uptr allocate_frame()
{
        return allocator_stage1.allocate(1) * FRAME_BYTES;
}
// --------------------------------------------------

// --------------------------------------------------
void free_frame(uptr addr)
{
        allocator_stage1.free(addr / FRAME_BYTES);
}
// --------------------------------------------------

} /* namespace kernel::mem::pmm */