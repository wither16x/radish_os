#pragma once

#include <lib/typing.hpp>

namespace kernel::mem::heap {

constexpr lib::uptr HEAP_START = 0xfffffe8000000000ull;
constexpr lib::uptr HEAP_PML4T_IDX = (HEAP_START >> 39) & 0x1ff;

/// Initialize the heap.
void init();
/// Allocate `n` bytes of memory on the heap.
/// Since the allocator it uses is based on a linked list,
/// it allocates `n + sizeof(BlockHeader)` bytes.
void *allocate(lib::usize n, bool debug);
/// Free the allocated memory area pointed by `p`.
void free(void *p);

} /* namespace kernel::mem::heap */