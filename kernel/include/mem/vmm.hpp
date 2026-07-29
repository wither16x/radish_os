#pragma once

#include <boot/bootinfo.hpp>
#include <lib/typing.hpp>
#include <mem/pml4t.hpp>

namespace kernel::mem::vmm {

/// Initialize the VMM.
PML4T init(
        lib::u64 hhdm_base,
        boot::BootInfo::ExecutableInfo &exec_info,
        boot::BootInfo::MemmapInfo &_memmap_info
);

} /* namespace kernel::mem::vmm */