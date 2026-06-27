#pragma once

#include <boot/bootinfo.hpp>
#include <lib/typing.hpp>

namespace kernel::mem::vmm {

constexpr lib::usize PAGE_BYTES = 0x1000;         // 4 KiB

/// Enumeration of some page flags.
enum PageFlag : lib::u64 {
        ReadWrite       = 0x03,
        ReadExecUser    = 0x05,
        NoExec          = 1ull << 63,
        ReadWriteUser   = 0x07,
        ReadExec        = 0x01
};

/// Initialize the VMM.
lib::u64 *init(
        lib::u64 hhdm_base,
        boot::BootInfo::ExecutableInfo &exec_info,
        boot::BootInfo::MemmapInfo &_memmap_info
);
/// Load a PML4 table into CR3.
void load(lib::u64 *pml4t);
/// Allocate a new PML4 table.
lib::u64 *create_pml4t(lib::u64 *parent);
/// Map a page in a given PML4 table.
void map_page(lib::u64 *pml4t, lib::uptr virt, lib::uptr phys, lib::u64 flags);
/// Unmap a page from a given PML4 table.
void unmap_page(lib::u64 *pml4t, lib::uptr virt);

// Helpers

inline lib::uptr page_align_down(lib::uptr base)
{
        return base / PAGE_BYTES * PAGE_BYTES;
}

inline lib::uptr page_align_up(lib::uptr base)
{
        return (base + PAGE_BYTES - 1) / PAGE_BYTES * PAGE_BYTES;
}

inline lib::uptr page_div_up(lib::uptr base)
{
        return (base + PAGE_BYTES - 1) / PAGE_BYTES;
}

} /* namespace kernel::mem::vmm */