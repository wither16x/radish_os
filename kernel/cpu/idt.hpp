#pragma once

#include <lib/typing.hpp>

namespace kernel::cpu {

class IDT {
public:
        IDT();

        void load();

private:
        void set_gate(int vector, void (*isr)(), lib::u8 flags);
};

} /* namespace kernel::cpu */