#pragma once

#include <lib/typing.hpp>
#include <mem/page.hpp>

namespace kernel::mem {

class PML4T {
        PageTable *raw_pml4t;

public:
        void init(this PML4T &self);
        void init(this PML4T &self, const PML4T &parent);
        
        void destroy(this PML4T &self);
        void load(this const PML4T &self);
        void map_page(this PML4T &self, lib::uptr vaddr, lib::uptr paddr, lib::u64 flags);
        void unmap_page(this PML4T &self, lib::uptr vaddr);

        PageTable *raw(this const PML4T &self);
};

} /* namespace kernel::mem */