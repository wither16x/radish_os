#pragma once

#include <cpu/tss.hpp>
#include <lib/typing.hpp>

namespace kernel::cpu {

/// Easy-to-use representation of a single GDT descriptor.
struct [[gnu::packed]] GDTDescriptor {
        lib::u16 limit_low;
        lib::u16 base_low;
        lib::u8  base_middle;
        lib::u8  access;
        lib::u8  limit_and_flags;
        lib::u8  base_high;
};

/// Easy-to-use representation of the GDT register.
struct [[gnu::packed]] GDTR {
        lib::u16 size;
        lib::u64 offset;
};

/// Object-oriented representation of a GDT.
class GDT {
        static constexpr int MAX_DESCRIPTORS = 7;

        TSS tss;
        GDTDescriptor descriptors[MAX_DESCRIPTORS];
        GDTR gdtptr;

        /// Edit a descriptor in the table.
        /// Note that 64-bit addresses should take 2 entries.
        void set_descriptor(this GDT &self, int n, lib::u32 base, lib::u32 limit, lib::u8 access, lib::u8 flags);

public:
        void init(this GDT &self);
        /// Load the GDT.
        void load(this GDT &self);

        TSS &get_tss(this GDT &self);
};

} /* namespace kernel::cpu */