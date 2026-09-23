#include <lib/alloc.hpp>
#include <mem/heap.hpp>
#include <kernel.hpp>
#include <panic.hpp>

void *operator new(size_t size)
{
        if (not Kiwi::kcontext().heap_available)
                Kiwi::panic("no heap available");

        void *p = Kiwi::Mem::Heap::allocate(size);
        return p;
}

void operator delete(void *ptr)
{
        if (ptr)
                Kiwi::Mem::Heap::free(ptr);
}

void *operator new[](size_t size)
{
        if (not Kiwi::kcontext().heap_available)
                Kiwi::panic("no heap available");

        return Kiwi::Mem::Heap::allocate(size);
}

void operator delete[](void *ptr)
{
        if (ptr)
                Kiwi::Mem::Heap::free(ptr);
}