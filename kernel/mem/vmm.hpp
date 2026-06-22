#pragma once

#include <boot/bootinfo.hpp>
#include <lib/typing.hpp>

namespace kernel::mem::vmm {

constexpr lib::usize PAGE_BYTES = 0x1000;         // 4 KiB

enum PageFlag : lib::u64 {
        ReadWrite       = 0x03,
        ReadExec        = 0x05,
        NoExec          = 1ull << 63
};

lib::u64 *init(
        lib::u64 hhdm_base,
        boot::BootInfo::ExecutableInfo &exec_info,
        boot::BootInfo::MemmapInfo &_memmap_info
);
void load(lib::u64 *pml4t);
lib::u64 *create_pml4t(lib::u64 *parent);

void map_page(lib::u64 *pml4t, lib::uptr virt, lib::uptr phys, lib::u64 flags);
void unmap_page(lib::u64 *pml4t, lib::uptr virt);

// Helpers
// --------------------------------------------------------
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
// --------------------------------------------------------

} /* namespace kernel::mem::vmm */