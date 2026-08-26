#pragma once

#include <lib/typing.hpp>

namespace Kiwi::Fpu
{
        /// stX and mmX
        struct RegisterStMm
        {
                Lib::u8 bytes[10];
                Lib::u8 reserved[6];
        };

        /// xmmX
        struct RegisterXmm
        {
                Lib::u8 bytes[16];
        };

        /// According to FXSAVE
        struct [[gnu::packed, gnu::aligned(16)]] FpuContext
        {
                Lib::u16 fcw;
                Lib::u16 fsw;
                Lib::u8 ftw;
                Lib::u8 __reserved0;
                Lib::u16 fop;
                Lib::u64 fip;
                Lib::u64 fdp;
                Lib::u32 mxcsr;
                Lib::u32 mxcsr_mask;
                RegisterStMm st_mm[8];
                RegisterXmm xmm[16];
                Lib::u8 __reserved1[96];
        };
} // namespace Kiwi::Fpu