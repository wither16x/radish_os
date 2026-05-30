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

        self.stage2_enabled = false;

        ok();
}

void PMM::init_stage2(this PMM &self)
{
        status("initializing pmm stage 2");

        self.dynamic_bitmap.init(self.static_bitmap.size());
        self.dynamic_bitmap.set_all();

        for (usize i = 0; i < self.dynamic_bitmap.size(); i++) {
                if (self.static_bitmap.test(i))
                        self.dynamic_bitmap.set(i);
        }

        self.stage2_enabled = true;

        ok();
}

uptr PMM::allocate_frame(this PMM &self)
{
        usize start = self.last_frame;

        if (self.stage2_enabled) {
                do {
                        if (!self.dynamic_bitmap.test(self.last_frame))
                                break;
                        
                        self.last_frame++;

                        if (self.last_frame >= self.dynamic_bitmap.size())
                                self.dynamic_bitmap.extend();
                } while (self.last_frame != start);

                self.dynamic_bitmap.set(self.last_frame);
        } else {
                do {
                        if (!self.static_bitmap.test(self.last_frame))
                                break;
                        
                        self.last_frame++;

                        if (self.last_frame >= self.MaxFrames)
                                panic("out of physical memory");
                } while (self.last_frame != start);

                self.static_bitmap.set(self.last_frame);
        }

        return self.last_frame * self.FrameBytes;
}

void PMM::free_frame(this PMM &self, uptr addr)
{
        uptr frame = addr / self.FrameBytes;

        if (self.stage2_enabled) {
                if (!self.dynamic_bitmap.test(frame))
                        panic("pmm: double free on frame at address 0x%x", addr);
                self.dynamic_bitmap.clear(frame);
        } else {
                if (!self.static_bitmap.test(frame))
                        panic("pmm: double free at address 0x%x", addr);
                self.static_bitmap.clear(frame);
        }
}

} /* namespace kernel::mem */