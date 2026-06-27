#pragma once

#include <lib/typing.hpp>

namespace kernel::drivers::pic {

/// Remap the PIC at automatically given offsets.
void remap();
/// Send "END OF INTERRUPT" to an IRQ.
void send_eoi(lib::u8 irq);
/// Disable an IRQ.
void irq_mask(lib::u8 irq);
/// Enable an IRQ.
void irq_unmask(lib::u8 irq);
/// Disable all IRQs.
void irq_mask_all();
/// Enable all IRQs.
void irq_unmask_all();

} /* namespace kernel::drivers::pic */