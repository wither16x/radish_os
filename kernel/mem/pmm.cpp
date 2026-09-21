#include <boot/requests.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>
#include <mem/allocators/static_bitmap.hpp>
#include <mem/pmm.hpp>
#include <kernel.hpp>

namespace Kiwi::Mem::Pmm
{
        namespace
        {
                constexpr Lib::usize FRAME_BYTES  = 0x1000;       // 4 KiB
                constexpr Lib::usize MAX_MEMORY   = 0x40000000;   // 1 GiB
                constexpr Lib::usize MAX_FRAMES   = MAX_MEMORY / FRAME_BYTES;

                Allocators::StaticBitmapAllocator<Lib::uptr, MAX_FRAMES> allocator;
        } // anonymous namespace

        void init()
        {
                const Boot::MemmapRequest &req_memmap = kcontext().bootloader.request<Boot::MemmapRequest>();

                allocator.getBitmap().setAll();

                for (Lib::usize i = 0; i < req_memmap.entry_count; i++) {
                        if (req_memmap.entries[i].type == Boot::MemmapEntryType::Usable) {
                                const Boot::MemmapEntry &e = req_memmap.entries[i];
                                for (Lib::uptr addr = e.base; addr < e.base + e.length; addr += FRAME_BYTES)
                                        allocator.getBitmap().clear(addr / FRAME_BYTES);
                        }
                }

                kcontext().logger.ok("initialized pmm");
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