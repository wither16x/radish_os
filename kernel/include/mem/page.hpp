#pragma once

#include <lib/typing.hpp>

namespace kernel::mem {

constexpr lib::usize PAGE_SIZE = 0x1000; // 4096 bytes
constexpr lib::usize PHYS_ADDR_MASK = ((1ull << 52) - 1) & ~0xfffull;
constexpr lib::u16 PAGE_TABLE_ENTRIES = 512;

/// Enumeration of some page flags.
enum PageFlag : lib::u64 {
        Present         = 0x01,
        ReadWrite       = 0x03,
        ReadExecUser    = 0x05,
        NoExec          = 1ull << 63,
        ReadWriteUser   = 0x07,
        ReadExec        = 0x01
};

struct [[gnu::packed]] PageTable {
        lib::u64 entries[PAGE_TABLE_ENTRIES];
};

inline lib::uptr page_align_down(lib::uptr base)
{
        return base / PAGE_SIZE * PAGE_SIZE;
}

inline lib::uptr page_align_up(lib::uptr base)
{
        return (base + PAGE_SIZE - 1) / PAGE_SIZE * PAGE_SIZE;
}

inline lib::uptr page_div_up(lib::uptr base)
{
        return (base + PAGE_SIZE - 1) / PAGE_SIZE;
}

} /* namespace kernel::mem */