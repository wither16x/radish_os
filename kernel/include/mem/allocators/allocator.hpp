#pragma once

#include <lib/typing.hpp>

namespace kernel::mem::allocators {

/// Base class for every allocator.
template<typename T>
class Allocator {
public:
        /// Allocate (usually) `n` bytes of memory.
        /// However, allocators do not always allocate
        /// bytes so the value represented by `n` is not
        /// fixed.
        virtual T allocate(lib::usize n) {(void)n;};
        /// Free (usually, see documentation for `allocate()`)
        /// a pointer to an allocated area.
        virtual void free(T n) {(void)n;};
};

} /* namespace kernel::mem::allocators */