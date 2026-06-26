#pragma once

#include <boot/bootinfo.hpp>
#include <lib/bitmap.hpp>

namespace kernel::mem::pmm {

/// Initialize the first stage of the PMM.
/// In this stage, the PMM can handle a maximum amount of
/// 2 GiB of RAM.
void init_stage1(boot::BootInfo::MemmapInfo &memmap);
/// Initialize the second stage of the PMM.
/// In this stage, the PMM can handle a lot of memory since
/// the buffer it uses to store the frames can extend itself
/// if needed.
/// NOTE: heap allocator required.
void init_stage2();
/// Allocate a frame and return its physical address.
lib::uptr allocate_frame();
/// Free an allocated frame based on its physical address.
void free_frame(lib::uptr addr);

} /* namespace kernel::mem::pmm */