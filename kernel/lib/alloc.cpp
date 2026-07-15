#include <lib/alloc.hpp>
#include <mem/heap.hpp>

namespace kernel::lib {

// --------------------------------------------------
void *malloc(usize n)
{
        return mem::heap::allocate(n);
}
// --------------------------------------------------

// --------------------------------------------------
void free(void *p)
{
        mem::heap::free(p);
}
// --------------------------------------------------

} /* namespace kernel::leab */

// --------------------------------------------------
void *operator new(size_t size)
{
        void *p = kernel::lib::malloc(size);
        return p;
}
// --------------------------------------------------

// --------------------------------------------------
void operator delete(void *ptr)
{
        return kernel::lib::free(ptr);
}
// --------------------------------------------------

// --------------------------------------------------
void *operator new[](size_t size)
{
        return kernel::lib::malloc(size);
}
// --------------------------------------------------

// --------------------------------------------------
void operator delete[](void *ptr)
{
        kernel::lib::free(ptr);
}
// --------------------------------------------------