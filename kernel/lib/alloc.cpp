#include <lib/alloc.hpp>
#include <mem/heap.hpp>

namespace Kiwi::Lib
{
        void *malloc(usize n)
        {
                return Mem::Heap::allocate(n);
        }

        void free(void *p)
        {
                Mem::Heap::free(p);
        }
} // namespace Kiwi::Lib

void *operator new(size_t size)
{
        void *p = Kiwi::Lib::malloc(size);
        return p;
}

void operator delete(void *ptr) noexcept
{
        Kiwi::Lib::free(ptr);
}

void *operator new[](size_t size)
{
        return Kiwi::Lib::malloc(size);
}

void operator delete[](void *ptr) noexcept
{
        Kiwi::Lib::free(ptr);
}