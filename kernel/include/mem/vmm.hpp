#pragma once

#include <lib/typing.hpp>
#include <mem/pml4t.hpp>

namespace Kiwi::Mem::Vmm
{
        /// Initialize the VMM.
        PML4T init();
} // namespace Kiwi::Mem::Vmm