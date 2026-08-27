#pragma once

#include <cpu/tss.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Cpu
{
        enum class Segment
        {
                KernelCs                = 0x08,
                KernelDs                = 0x10,
                UserCs                  = 0x1b,
                UserSs                  = 0x23
        };

        /// Easy-to-use representation of a single GDT descriptor.
        struct [[gnu::packed]] GdtDescriptor
        {
                Lib::u16 limit_low;
                Lib::u16 base_low;
                Lib::u8  base_middle;
                Lib::u8  access;
                Lib::u8  limit_and_flags;
                Lib::u8  base_high;
        };

        /// Easy-to-use representation of the GDT register.
        struct [[gnu::packed]] Gdtr
        {
                Lib::u16 size;
                Lib::u64 offset;
        };

        /// Object-oriented representation of a GDT.
        class Gdt
        {
                static constexpr int MAX_DESCRIPTORS = 7;

                Tss tss;
                GdtDescriptor descriptors[MAX_DESCRIPTORS];
                Gdtr gdtptr;

                /// Edit a descriptor in the table.
                /// Note that 64-bit addresses should take 2 entries.
                void setDescriptor(this Gdt &self, int n, Lib::u32 base, Lib::u32 limit, Lib::u8 access, Lib::u8 flags);

        public:
                void init(this Gdt &self);
                /// Load the GDT.
                void load(this Gdt &self);

                Tss &getTss(this Gdt &self);
        };
} // namespace Kiwi::Cpu