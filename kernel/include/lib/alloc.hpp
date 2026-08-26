#pragma once

#include <lib/typing.hpp>

namespace Kiwi::Lib
{
        /// Allocate `n` bytes on the heap and return a pointer to the
        /// base address of the allocated area.
        void *malloc(usize n);
        /// Free a pointer which has been previously allocated on the
        /// heap.
        void free(void *p);
} // namespace Kiwi::Lib

// `new` and `delete` cannot be declared into a namespace so they are declared
// here.
// We also do not use kernel standard types here to not make the compiler scream
// of pain.

void *operator new(size_t size);
void operator delete(void *ptr) noexcept;

void *operator new[](size_t size);
void operator delete[](void *ptr) noexcept;

// below, placement new/delete

inline void *operator new(size_t, void *p) noexcept
{
        return p;
}

inline void operator delete(void *, void *) noexcept;

inline void *operator new[](size_t, void *p) noexcept
{
        return p;
}

inline void operator delete[](void *, void *) noexcept;