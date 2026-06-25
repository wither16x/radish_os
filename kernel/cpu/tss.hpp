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

void init_tss(TSS *tss, lib::uptr krsp);
// defined in tss_flush.asm
extern "C" void tss_flush();

} /* namespace kernel::cpu */