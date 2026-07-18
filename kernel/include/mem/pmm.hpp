#pragma once

#include <boot/bootinfo.hpp>
#include <lib/bitmap.hpp>

namespace kernel::mem::pmm {

/// Initialize the PMM.
/// The PMM can handle a maximum amount of 2 GiB of RAM.
void init(boot::BootInfo::MemmapInfo &memmap);
/// Allocate a frame and return its physical address.
lib::uptr allocate_frame();
/// Free an allocated frame based on its physical address.
void free_frame(lib::uptr addr);

} /* namespace kernel::mem::pmm */