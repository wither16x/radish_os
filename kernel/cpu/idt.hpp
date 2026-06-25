#pragma once

#include <lib/typing.hpp>

namespace kernel::cpu {

/// Object-oriented representation of an IDT, even though it
/// uses an implicit state which is owned by the source file
/// and not by the class itself.
class IDT {
public:
        IDT();

        /// Load the IDT.
        void load();

private:
        /// Edit a gate in the table.
        void set_gate(int vector, void (*isr)(), lib::u8 flags);
};

} /* namespace kernel::cpu */