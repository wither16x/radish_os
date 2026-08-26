#pragma once

#include <lib/typing.hpp>
#include <mem/page.hpp>

namespace Kiwi::Mem
{
        class PML4T {
                PageTable *raw_pml4t;

        public:
                void init(this PML4T &self);
                void init(this PML4T &self, const PML4T &parent);
                
                void destroy(this PML4T &self);
                void load(this const PML4T &self);
                void mapPage(this PML4T &self, Lib::uptr vaddr, Lib::uptr paddr, Lib::u64 flags);
                void unmapPage(this PML4T &self, Lib::uptr vaddr);
                Lib::uptr virtToPhys(this const PML4T &self, Lib::uptr vaddr);

                bool isMapped(this const PML4T &self, Lib::uptr vaddr);

                PageTable *raw(this const PML4T &self);
        };
} // namespace Kiwi::Mem