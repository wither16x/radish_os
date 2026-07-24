#pragma once

#include <lib/typing.hpp>

namespace kernel::cpu {

/// Easy-to-use representation of a single IDT entry.
struct [[gnu::packed]] IDTEntry {
        lib::u16 isr_low;
        lib::u16 selector;
        lib::u8  ist;        // 3 bits for the IST and the 5 other bits are reserved
        lib::u8  flags;      // gate type + 0 + dpl + p
        lib::u16 isr_mid;
        lib::u32 isr_high;
        lib::u32 __reserved;
};

/// Easy-to-use representation of the IDT register.
struct [[gnu::packed]] IDTR {
        lib::u16 size;
        lib::u64 offset;
};

/// Object-oriented representation of an IDT.
class IDT {
        static constexpr int MAX_GATES          = 255;
        static constexpr lib::u16 SEGMENT_SELECTOR   = 0x08;

        IDTEntry gates[MAX_GATES];
        IDTR idtptr;

        /// Edit a gate in the table.
        void set_gate(this IDT &self, int vector, void (*isr)(), lib::u8 flags);

public:
        void init(this IDT &self);
        /// Load the IDT.
        void load(this IDT &self);
};

} /* namespace kernel::cpu */