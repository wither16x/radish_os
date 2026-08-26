#pragma once

#include <lib/typing.hpp>

namespace Kiwi::Drivers::Pic
{
        enum IrqType : Lib::u64
        {
                IRQ_TIMER,
                IRQ_KEYBOARD
        };

        /// Remap the PIC at automatically given offsets.
        void remap();
        /// Send "END OF INTERRUPT" to an IRQ.
        void sendEoi(Lib::u8 irq);
        /// Disable an IRQ.
        void irqMask(Lib::u8 irq);
        /// Enable an IRQ.
        void irqUnmask(Lib::u8 irq);
        /// Disable all IRQs.
        void irqMaskAll();
        /// Enable all IRQs.
        void irqUnmaskAll();
} // namespace Kiwi::Drivers::Pic