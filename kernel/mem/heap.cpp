#include <lib/logging.hpp>
#include <lib/typing.hpp>
#include <mem/allocators/heap.hpp>
#include <mem/heap.hpp>
#include <kernel.hpp>

namespace Kiwi::Mem::Heap
{
        namespace
        {
                Allocators::HeapAllocator<void *> allocator;
        } // anonymous namespace

        void init()
        {
                allocator.init(HEAP_START, 16);
                kcontext.heap_available = true;
                kcontext.logger.ok("initialized heap");
        }

        void *allocate(Lib::usize n)
        {
                return allocator.allocate(n);
        }

        void free(void *p)
        {
                allocator.free(p);
        }
} // namespace Kiwi::Mem::Heap