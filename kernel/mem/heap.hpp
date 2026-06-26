#pragma once

#include <lib/typing.hpp>

namespace kernel::mem::heap {

/// Initialize the heap.
void init();
/// Allocate `n` bytes of memory on the heap.
/// Since the allocator it uses is based on a linked list,
/// it allocates `n + sizeof(BlockHeader)` bytes.
void *allocate(lib::usize n);
/// Free the allocated memory area pointed by `p`.
void free(void *p);

} /* namespace kernel::mem::heap */