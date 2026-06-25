#include <cpu/tss.hpp>
#include <lib/typing.hpp>

namespace kernel::cpu {

void init_tss(TSS *tss, lib::uptr krsp)
{
        tss->rsp0 = krsp;
}

} /* namespace kernel::cpu */