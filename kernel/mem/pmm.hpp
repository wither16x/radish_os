#pragma once

#include <boot/bootinfo.hpp>
#include <lib/bitmap.hpp>

namespace kernel::mem {

class PMM {
public:
        PMM(boot::boot_info::memmap_info &memmap);

        lib::uptr allocate_frame(this PMM &self);
        void free_frame(this PMM &self, lib::uptr addr);

private:
        static constexpr lib::usize MaxMemory   = 0x40000000;   // 1 GiB
        static constexpr lib::usize FrameBytes  = 0x1000;       // 4 KiB

        lib::StaticBitmap<MaxMemory / FrameBytes> bitmap;
        lib::usize last_frame                   = 0;            // last allocated frame
};

} /* namespace kernel::mem */