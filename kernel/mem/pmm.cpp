#include <boot/bootinfo.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>
#include <mem/pmm.hpp>
#include <panic.hpp>

using kernel::lib::log::status, kernel::lib::log::ok;
using kernel::lib::usize, kernel::lib::uptr, kernel::lib::u64;

namespace kernel::mem {

void PMM::init_stage1(this PMM &self, boot::BootInfo::MemmapInfo &memmap)
{
        status("initializing pmm stage 1");

        self.static_bitmap.set_all();

        for (usize i = 0; i < memmap.entry_count; i++) {
                if (memmap.entries[i].type == boot::MemmapEntryType::Usable) {
                        boot::memmap_entry& e = memmap.entries[i];
                        for (uptr addr = e.base; addr < e.base + e.length; addr += self.FrameBytes)
                                self.static_bitmap.clear(addr / self.FrameBytes);
                }
        }

        ok();
}

uptr PMM::allocate_frame(this PMM &self)
{
        usize start = self.last_frame;

        do {
                if (!self.static_bitmap.test(self.last_frame))
                        break;
                
                self.last_frame++;

                if (self.last_frame >= self.MaxFrames)
                        panic("out of physical memory");
        } while (self.last_frame != start);

        self.static_bitmap.set(self.last_frame);

        return self.last_frame * self.FrameBytes;
}

void PMM::free_frame(this PMM &self, uptr addr)
{
        uptr frame = addr / self.FrameBytes;

        if (!self.static_bitmap.test(frame))
                panic("pmm: double free at address 0x%x", addr);
        self.static_bitmap.clear(frame);
}

} /* namespace kernel::mem */