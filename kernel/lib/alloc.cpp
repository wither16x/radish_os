#include <lib/alloc.hpp>
#include <mem/heap.hpp>
#include <kernel.hpp>
#include <panic.hpp>

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
        if (not Kiwi::kcontext().heap_available)
                Kiwi::panic("no heap available");

        void *p = Kiwi::Lib::malloc(size);
        return p;
}

void operator delete(void *ptr) noexcept
{
        if (ptr)
                Kiwi::Lib::free(ptr);
}

void *operator new[](size_t size)
{
        if (not Kiwi::kcontext().heap_available)
                Kiwi::panic("no heap available");

        return Kiwi::Lib::malloc(size);
}

void operator delete[](void *ptr) noexcept
{
        if (ptr)
                Kiwi::Lib::free(ptr);
}