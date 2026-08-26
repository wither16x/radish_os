#pragma once

#include <boot/bootinfo.hpp>
#include <lib/typing.hpp>
#include <mem/pml4t.hpp>

namespace Kiwi::Mem::Vmm
{
        /// Initialize the VMM.
        PML4T init(
                Lib::u64 hhdm_base,
                Boot::BootInfo::ExecutableInfo &exec_info,
                Boot::BootInfo::MemmapInfo &_memmap_info
        );
} // namespace Kiwi::Mem::Vmm