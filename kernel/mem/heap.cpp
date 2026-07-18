#include <lib/logging.hpp>
#include <lib/typing.hpp>
#include <mem/allocators/heap.hpp>
#include <mem/heap.hpp>

using kernel::lib::uptr;
using kernel::lib::log::logger;

namespace kernel::mem::heap {

namespace {

allocators::HeapAllocator<void *> allocator;

} /* anonymous namespace */

// --------------------------------------------------
void init()
{
        allocator.init(HEAP_START, 16);
        logger.ok("initialized heap");
}
// --------------------------------------------------

// --------------------------------------------------
void *allocate(lib::usize n)
{
        return allocator.allocate(n);
}
// --------------------------------------------------

// --------------------------------------------------
void free(void *p)
{
        allocator.free(p);
}
// --------------------------------------------------

} /* namespace kernel::mem::heap */