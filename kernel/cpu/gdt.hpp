#pragma once

#include <lib/typing.hpp>

namespace kernel {

namespace cpu {

class GDT {
public:
        GDT();
        void load();

private:
        void set_descriptor(int n, lib::u32 base, lib::u32 limit, lib::u8 access, lib::u8 flags);
};

} /* namespace cpu */

} /* namespace kernel */