#include <cpu/idt.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>

#define ISR(n)                  extern "C" void __isr_stub##n()
#define IRQ(n)                  extern "C" void __irq_stub##n()

using kernel::lib::u8, kernel::lib::u16, kernel::lib::u32, kernel::lib::u64;
using kernel::lib::log::logger;

namespace kernel::cpu {

namespace {

/// Reload the IDT.
extern "C" void __idt_flush(u64 *idtr);

// ISR stubs
ISR(3);
ISR(10);
ISR(13);
ISR(14);

// IRQ stubs
IRQ(0);
IRQ(1);

// Syscall
extern "C" void syscall_common();

} /* anonymous namespace */

// --------------------------------------------------
void IDT::init(this IDT &self)
{
        self.idtptr = {
                .size = sizeof(self.gates) - 1,
                .offset = reinterpret_cast<u64>(&self.gates)
        };

        // isr
        self.set_gate(3, __isr_stub3, 0x8e);
        self.set_gate(10, __isr_stub10, 0x8e);
        self.set_gate(13, __isr_stub13, 0x8e);
        self.set_gate(14, __isr_stub14, 0x8e);
        // irq
        self.set_gate(32, __irq_stub0, 0x8e);
        self.set_gate(33, __irq_stub1, 0x8e);
        // syscall
        self.set_gate(128, syscall_common, 0xef);

        logger.ok("initialized idt");
}
// --------------------------------------------------

// --------------------------------------------------
void IDT::load(this IDT &self)
{
        __idt_flush(reinterpret_cast<u64 *>(&self.idtptr));

        logger.ok("loaded idt");
}
// --------------------------------------------------

// --------------------------------------------------
void IDT::set_gate(this IDT &self, int vector, void (*isr)(), u8 flags)
{
        self.gates[vector].isr_low     = reinterpret_cast<u64>(isr) & 0xffff;
        self.gates[vector].selector    = SEGMENT_SELECTOR;
        self.gates[vector].ist         = 0;
        self.gates[vector].flags       = flags;
        self.gates[vector].isr_mid     = (reinterpret_cast<u64>(isr) >> 16) & 0xffff;
        self.gates[vector].isr_high    = (reinterpret_cast<u64>(isr) >> 32) & 0xffffffff;
        self.gates[vector].__reserved  = 0;
}
// --------------------------------------------------

} /* namespace kernel::cpu */