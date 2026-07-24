#include <cpu/tss.hpp>
#include <lib/typing.hpp>

using kernel::lib::uptr;

namespace kernel::cpu {

namespace {

extern "C" void __tss_flush();

} /* anonymous namespace */

// --------------------------------------------------
void TSS::init(this TSS &self, uptr kernel_rsp)
{
        self.data.rsp0 = kernel_rsp;
}
// --------------------------------------------------

void TSS::flush() const
{
        __tss_flush();
}

void TSS::reset_stack(this TSS &self, uptr kernel_rsp)
{
        self.data.rsp0 = kernel_rsp;
}

const TSSData &TSS::get_data(this const TSS &self)
{
        return self.data;
}

} /* namespace kernel::cpu */