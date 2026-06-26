#include <kernel.hpp>
#include <lib/typing.hpp>

using kernel::lib::u64;

namespace kernel {

namespace {

u64 *kpml4t = nullptr;
u64 hhdm_offset;

} /* anonymous namespace */

// --------------------------------------------------
u64 *get_kernel_pml4t()
{
        return kpml4t;
}
// --------------------------------------------------

// --------------------------------------------------
void set_kernel_pml4t(lib::u64 *pml4t)
{
        kpml4t = pml4t;
}
// --------------------------------------------------

// --------------------------------------------------
u64 get_kernel_hhdm_offset()
{
        return hhdm_offset;
}
// --------------------------------------------------

// --------------------------------------------------
void set_kernel_hhdm_offset(lib::u64 offset)
{
        hhdm_offset = offset;
}
// --------------------------------------------------

} /* namespace kernel */