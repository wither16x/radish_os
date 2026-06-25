#pragma once

#include <lib/typing.hpp>

namespace kernel::cpu {

struct [[gnu::packed]] TSS {
        lib::u32 __reserved0;
        lib::u64 rsp0;
        lib::u64 rsp1;
        lib::u64 rsp2;
        lib::u64 __useless[21];
};

/// Initialize a TSS with a pointer to the top of the kernel
/// stack.
void init_tss(TSS *tss, lib::uptr krsp);
/// Reload the task register.
extern "C" void tss_flush();

} /* namespace kernel::cpu */