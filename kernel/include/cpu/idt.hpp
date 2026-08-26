#pragma once

#include <lib/typing.hpp>

namespace Kiwi::Cpu
{
        /// Easy-to-use representation of a single IDT entry.
        struct [[gnu::packed]] IdtEntry
        {
                Lib::u16 isr_low;
                Lib::u16 selector;
                Lib::u8  ist;        // 3 bits for the IST and the 5 other bits are reserved
                Lib::u8  flags;      // gate type + 0 + dpl + p
                Lib::u16 isr_mid;
                Lib::u32 isr_high;
                Lib::u32 __reserved;
        };

        /// Easy-to-use representation of the IDT register.
        struct [[gnu::packed]] Idtr
        {
                Lib::u16 size;
                Lib::u64 offset;
        };

        /// Object-oriented representation of an IDT.
        class Idt
        {
                static constexpr int MAX_GATES = 255;
                static constexpr Lib::u16 SEGMENT_SELECTOR = 0x08;

                IdtEntry gates[MAX_GATES];
                Idtr idtptr;

                /// Edit a gate in the table.
                void setGate(this Idt &self, int vector, void (*isr)(), Lib::u8 flags);

        public:
                void init(this Idt &self);
                /// Load the IDT.
                void load(this Idt &self);
        };
} // namespace Kiwi::Cpu