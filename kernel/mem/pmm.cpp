#include <boot/bootinfo.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>
#include <mem/allocators/static_bitmap.hpp>
#include <mem/allocators/dynamic_bitmap.hpp>
#include <mem/pmm.hpp>
#include <panic.hpp>

using kernel::lib::log::logger;
using kernel::lib::usize, kernel::lib::uptr, kernel::lib::u64;

namespace kernel::mem::pmm {

namespace {

constexpr lib::usize FRAME_BYTES  = 0x1000;       // 4 KiB
// Both constants below are used for stage 1 only
constexpr lib::usize MAX_MEMORY   = 0x40000000;   // 1 GiB
constexpr lib::usize MAX_FRAMES   = MAX_MEMORY / FRAME_BYTES;

bool stage2_enabled = false;

allocators::StaticBitmapAllocator<uptr, MAX_FRAMES> allocator_stage1;
allocators::DynamicBitmapAllocator<uptr> allocator_stage2;

} /* anonymous namespace */

void init_stage1(boot::BootInfo::MemmapInfo &memmap)
{
        allocator_stage1.get_bitmap().set_all();

        for (usize i = 0; i < memmap.entry_count; i++) {
                if (memmap.entries[i].type == boot::MemmapEntryType::Usable) {
                        boot::MemmapEntry& e = memmap.entries[i];
                        for (uptr addr = e.base; addr < e.base + e.length; addr += FRAME_BYTES)
                                allocator_stage1.get_bitmap().clear(addr / FRAME_BYTES);
                }
        }

        stage2_enabled = false;

        logger.ok("initialized pmm stage 1");
}

void init_stage2()
{
        allocator_stage2.get_bitmap().init(allocator_stage1.get_bitmap().size());
        allocator_stage2.get_bitmap().set_all();

        for (usize i = 0; i < allocator_stage2.get_bitmap().size(); i++) {
                if (allocator_stage1.get_bitmap().test(i))
                        allocator_stage2.get_bitmap().set(i);
        }

        stage2_enabled = true;

        logger.ok("initialized pmm stage 2");
}

uptr allocate_frame()
{
        if (stage2_enabled)
                return allocator_stage2.allocate(1) * FRAME_BYTES;
        else
                return allocator_stage1.allocate(1) * FRAME_BYTES;
}

void free_frame(uptr addr)
{
        if (stage2_enabled)
                allocator_stage2.free(addr / FRAME_BYTES);
        else
                allocator_stage1.free(addr / FRAME_BYTES);
}

} /* namespace kernel::mem::pmm */