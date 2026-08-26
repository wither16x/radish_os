#pragma once

#include <cpu/assembly.hpp>

namespace Kiwi::Cpu
{
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
} // namespace Kiwi::Cpu