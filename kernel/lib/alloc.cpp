#include <lib/alloc.hpp>
#include <mem/heap.hpp>

namespace kernel::lib {

// --------------------------------------------------
void *malloc(usize n, bool debug)
{
        return mem::heap::allocate(n, debug);
}
// --------------------------------------------------

// --------------------------------------------------
void free(void *p)
{
        mem::heap::free(p);
}
// --------------------------------------------------

} /* namespace kernel::lib */

// --------------------------------------------------
void *operator new(size_t size)
{
        void *p = kernel::lib::malloc(size);
        return p;
}
// --------------------------------------------------

// --------------------------------------------------
void operator delete(void *ptr) noexcept
{
        kernel::lib::free(ptr);
}
// --------------------------------------------------

// --------------------------------------------------
void *operator new[](size_t size)
{
        return kernel::lib::malloc(size, kernel::lib::debug_new);
}
// --------------------------------------------------

// --------------------------------------------------
void operator delete[](void *ptr) noexcept
{
        kernel::lib::free(ptr);
}
// --------------------------------------------------