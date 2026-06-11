#pragma once

#include <boot/bootinfo.hpp>
#include <lib/bitmap.hpp>

namespace kernel::mem::pmm {

void init_stage1(boot::BootInfo::MemmapInfo &memmap);
void init_stage2();

lib::uptr allocate_frame();
void free_frame(lib::uptr addr);

} /* namespace kernel::mem::pmm */