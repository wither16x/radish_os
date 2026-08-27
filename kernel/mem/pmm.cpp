#include <boot/bootinfo.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>
#include <mem/allocators/static_bitmap.hpp>
#include <mem/allocators/dynamic_bitmap.hpp>
#include <mem/pmm.hpp>

namespace Kiwi::Mem::Pmm
{
        namespace
        {
                constexpr Lib::usize FRAME_BYTES  = 0x1000;       // 4 KiB
                constexpr Lib::usize MAX_MEMORY   = 0x40000000;   // 1 GiB
                constexpr Lib::usize MAX_FRAMES   = MAX_MEMORY / FRAME_BYTES;

                Allocators::StaticBitmapAllocator<Lib::uptr, MAX_FRAMES> allocator;
        } // anonymous namespace

        void init(Boot::BootInfo::MemmapInfo &memmap)
        {
                allocator.getBitmap().setAll();

                for (Lib::usize i = 0; i < memmap.entry_count; i++) {
                        if (memmap.entries[i].type == Boot::MemmapEntryType::Usable) {
                                Boot::MemmapEntry &e = memmap.entries[i];
                                for (Lib::uptr addr = e.base; addr < e.base + e.length; addr += FRAME_BYTES)
                                        allocator.getBitmap().clear(addr / FRAME_BYTES);
                        }
                }

                Lib::Log::logger.ok("initialized pmm");
        }

        Lib::uptr allocateFrame()
        {
                return allocator.allocate(1) * FRAME_BYTES;
        }

        void freeFrame(Lib::uptr addr)
        {
                allocator.free(addr / FRAME_BYTES);
        }

} // namespace Kiwi::Mem::Pmm