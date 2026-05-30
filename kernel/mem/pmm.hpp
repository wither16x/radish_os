#pragma once

#include <boot/bootinfo.hpp>
#include <lib/bitmap.hpp>

namespace kernel::mem {

class PMM {
public:
        // (I'm too lazy to implement stage 2 for now.........)
        //
        // The PMM has 2 stages:
        // * stage 1: can handle 1GiB of RAM at max since the bitmap is statically
        //   allocated
        // * stage 2 (to be initialized when a heap allocator is available):
        //   can handle a theorically infinite amount of RAM since the bitmap
        //   is dynamically allocated
        void init_stage1(this PMM &self, boot::BootInfo::MemmapInfo &memmap);

        lib::uptr allocate_frame(this PMM &self);
        void free_frame(this PMM &self, lib::uptr addr);

private:
        static constexpr lib::usize FrameBytes  = 0x1000;       // 4 KiB
        // Both constants below are used for stage 1 and stage 2 initialization
        // only
        static constexpr lib::usize MaxMemory   = 0x40000000;   // 1 GiB
        static constexpr lib::usize MaxFrames   = MaxMemory / FrameBytes;

        lib::StaticBitmap<MaxFrames> static_bitmap;        // used in stage 1

        lib::usize last_frame                   = 0;            // last allocated frame
};

inline PMM pmm;

} /* namespace kernel::mem */