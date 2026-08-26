#pragma once

#include <lib/typing.hpp>

namespace Kiwi::Mem::Heap
{
        constexpr Lib::uptr HEAP_START = 0xfffffe8000000000ull;
        constexpr Lib::uptr HEAP_PML4T_IDX = (HEAP_START >> 39) & 0x1ff;

        /// Initialize the heap.
        void init();
        /// Allocate `n` bytes of memory on the heap.
        /// Since the allocator it uses is based on a linked list,
        /// it allocates `n + sizeof(BlockHeader)` bytes.
        void *allocate(Lib::usize n);
        /// Free the allocated memory area pointed by `p`.
        void free(void *p);
} // namespace Kiwi::Mem::Heap