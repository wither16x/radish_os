#pragma once

#include <lib/typing.hpp>

namespace kernel::drivers::pic {

void remap();
void send_eoi(lib::u8 irq);
void irq_mask(lib::u8 irq);
void irq_unmask(lib::u8 irq);
void irq_mask_all();
void irq_unmask_all();

} /* namespace kernel::drivers::pic */