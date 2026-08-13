#pragma once

#include <lib/typing.hpp>

namespace kernel::fpu {

/// stX and mmX
struct RegisterStMm {
        lib::u8 bytes[10];
        lib::u8 reserved[6];
};

/// xmmX
struct RegisterXmm {
        lib::u8 bytes[16];
};

/// According to FXSAVE
struct [[gnu::packed, gnu::aligned(16)]] FpuContext {
        lib::u16 fcw;
        lib::u16 fsw;
        lib::u8 ftw;
        lib::u8 __reserved0;
        lib::u16 fop;
        lib::u64 fip;
        lib::u64 fdp;
        lib::u32 mxcsr;
        lib::u32 mxcsr_mask;
        RegisterStMm st_mm[8];
        RegisterXmm xmm[16];
        lib::u8 __reserved1[96];
};

} // namespace kernel::fpu