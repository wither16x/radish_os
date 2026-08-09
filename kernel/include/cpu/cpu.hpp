#pragma once

#include <cpu/assembly.hpp>

namespace kernel::cpu {

inline void idle()
{
        hlt();
}

inline void disable_interrupts()
{
        cli();
}

inline void enable_interrupts()
{
        sti();
}

} // namespace kernel::cpu