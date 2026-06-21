#pragma once

#include <lib/typing.hpp>

namespace kernel::lib {

void *malloc(usize n);
void free(void *p);

} /* namespace kernel::lib */

// `new` and `delete` cannot be declared into a namespace so they are declared
// here.
// We also do not use kernel standard types here to not make the compiler scream
// of pain.

void *operator new(size_t size);
void operator delete(void *ptr);

void *operator new[](size_t size);
void operator delete[](void *ptr);

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