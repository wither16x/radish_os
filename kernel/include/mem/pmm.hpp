#pragma once

#include <boot/bootinfo.hpp>
#include <lib/bitmap.hpp>

namespace Kiwi::Mem::Pmm
{
        /// Initialize the PMM.
        /// The PMM can handle a maximum amount of 2 GiB of RAM.
        void init(Boot::BootInfo::MemmapInfo &memmap);
        /// Allocate a frame and return its physical address.
        Lib::uptr allocateFrame();
        /// Free an allocated frame based on its physical address.
        void freeFrame(Lib::uptr addr);
} // namespace Kiwi::Mem::Pmm