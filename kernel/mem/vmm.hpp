#pragma once

#include <boot/bootinfo.hpp>
#include <lib/typing.hpp>

namespace kernel::mem::vmm {

constexpr lib::usize PageBytes = 0x1000;         // 4 KiB

void init(
        lib::u64 hhdm_base,
        boot::BootInfo::ExecutableInfo &exec_info,
        boot::BootInfo::MemmapInfo &_memmap_info
);
void load();

void map_page(lib::uptr virt, lib::uptr phys, lib::u64 flags);
void unmap_page(lib::uptr virt);

// Helpers
// --------------------------------------------------------
inline lib::uptr page_align_down(lib::uptr base)
{
        return base / PageBytes * PageBytes;
}

inline lib::uptr page_align_up(lib::uptr base)
{
        return (base + PageBytes - 1) / PageBytes * PageBytes;
}

inline lib::uptr page_div_up(lib::uptr base)
{
        return (base + PageBytes - 1) / PageBytes;
}
// --------------------------------------------------------

} /* namespace kernel::mem::vmm */