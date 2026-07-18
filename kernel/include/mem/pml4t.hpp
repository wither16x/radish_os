#pragma once

#include <lib/typing.hpp>

namespace kernel::mem {

class PML4T {
        lib::u64 *raw_pml4t;

public:
        void init(this PML4T &self);
        void init(this PML4T &self, const PML4T &parent);
        
        void destroy(this PML4T &self);
        void load(this const PML4T &self);
        void map_page(this PML4T &self, lib::uptr vaddr, lib::uptr paddr, lib::u64 flags);
        void unmap_page(this PML4T &self, lib::uptr vaddr);

        lib::u64 *raw(this const PML4T &self);
};

} /* namespace kernel::mem */