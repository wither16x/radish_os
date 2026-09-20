#pragma once

#include <boot/requests.hpp>
#include <lib/typing.hpp>
#include <mem/pml4t.hpp>

namespace Kiwi::Mem::Vmm
{
        /// Initialize the VMM.
        PML4T init(
                Lib::u64 hhdm_base,
                const Boot::ExecutableAddressRequest &exec_info,
                const Boot::MemmapRequest &_memmap_info
        );
} // namespace Kiwi::Mem::Vmm