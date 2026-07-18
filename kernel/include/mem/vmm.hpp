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
// /// Load a PML4 table into CR3.
// void load(lib::u64 *pml4t);
// /// Allocate a new PML4 table.
// lib::u64 *create_pml4t(lib::u64 *parent);
// /// Free an existing PML4 table.
// void destroy_pml4t(lib::u64 *pml4t);
// /// Map a page in a given PML4 table.
// void map_page(lib::u64 *pml4t, lib::uptr virt, lib::uptr phys, lib::u64 flags);
// /// Unmap a page from a given PML4 table.
// void unmap_page(lib::u64 *pml4t, lib::uptr virt);

} /* namespace kernel::mem::vmm */