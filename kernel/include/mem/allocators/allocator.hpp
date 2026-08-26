#pragma once

#include <lib/typing.hpp>

namespace Kiwi::Mem::Allocators
{
        /// Base class for every allocator.
        template<typename T>
        class Allocator
        {
        public:
                /// Allocate (usually) `n` bytes of memory.
                /// However, allocators do not always allocate
                /// bytes so the value represented by `n` is not
                /// fixed.
                virtual T allocate(Lib::usize n) = 0;
                /// Free (usually, see documentation for `allocate()`)
                /// a pointer to an allocated area.
                virtual void free(T n) = 0;
        };
} // namespace Kiwi::Mem::Allocators