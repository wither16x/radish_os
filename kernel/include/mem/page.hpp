#pragma once

#include <lib/typing.hpp>

namespace Kiwi::Mem
{
        constexpr Lib::usize PAGE_SIZE = 0x1000; // 4096 bytes
        constexpr Lib::usize PHYS_ADDR_MASK = ((1ull << 52) - 1) & ~0xfffull;
        constexpr Lib::u16 PAGE_TABLE_ENTRIES = 512;

        /// Enumeration of some page flags.
        enum PageFlag : Lib::u64
        {
                Present         = 0x01,
                ReadWrite       = 0x03,
                ReadExecUser    = 0x05,
                NoExec          = 1ull << 63,
                ReadWriteUser   = 0x07,
                ReadExec        = 0x01
        };

        struct [[gnu::packed]] PageTable
        {
                Lib::u64 entries[PAGE_TABLE_ENTRIES];
        };

        Lib::uptr ptDeepCopy(PageTable *src, int level);

        inline Lib::uptr pageAlignDown(Lib::uptr base)
        {
                return base / PAGE_SIZE * PAGE_SIZE;
        }

        inline Lib::uptr pageAlignUp(Lib::uptr base)
        {
                return (base + PAGE_SIZE - 1) / PAGE_SIZE * PAGE_SIZE;
        }

        inline Lib::uptr pageDivUp(Lib::uptr base)
        {
                return (base + PAGE_SIZE - 1) / PAGE_SIZE;
        }
} // namespace Kiwi::Mem