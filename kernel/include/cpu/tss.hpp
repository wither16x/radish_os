#pragma once

#include <lib/typing.hpp>

namespace Kiwi::Cpu
{
        /// Representation of a task state segment.
        /// Note that we only use RSP0.
        struct [[gnu::packed]] TssData
        {
                Lib::u32 __reserved0;
                Lib::u64 rsp0;
                Lib::u64 rsp1;
                Lib::u64 rsp2;
                Lib::u64 __reserved1;
                Lib::u64 ist[7];
                Lib::u64 __reserved2;
                Lib::u64 __reserved3;
                Lib::u16 iobp;
        };

        class Tss
        {
                TssData data;

        public:
                void init(this Tss &self, Lib::uptr kernel_rsp);
                void flush() const;
                void resetStack(this Tss &self, Lib::uptr kernel_rsp);

                const TssData &getData(this const Tss &self);
        };
} // namespace Kiwi::Cpu