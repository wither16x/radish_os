#pragma once

#include <lib/typing.hpp>

namespace kernel::cpu {

/// Object-oriented representation of a GDT, even though it
/// uses an implicit state which is owned by the source file
/// and not by the class itself.
class GDT {
public:
        /// Initialize the GDT but do not load it, as this
        /// should be done by the user (I mean the one who
        /// calls the function: user, programmer... call it
        /// however you want).
        GDT();
        
        /// Load the GDT.
        void load();

private:
        /// Edit a descriptor in the table.
        /// Note that 64-bit addresses should take 2 entries.
        void set_descriptor(int n, lib::u32 base, lib::u32 limit, lib::u8 access, lib::u8 flags);
};

} /* namespace kernel::cpu */