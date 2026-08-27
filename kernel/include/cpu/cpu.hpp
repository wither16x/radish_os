#pragma once

#include <cpu/assembly.hpp>

namespace Kiwi::Cpu
{
        inline void idle()
        {
                hlt();
        }

        inline void disableInterrupts()
        {
                cli();
        }

        inline void enableInterrupts()
        {
                sti();
        }
} // namespace Kiwi::Cpu