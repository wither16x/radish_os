#include <cpu/idt.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>

using kernel::lib::u8, kernel::lib::u16, kernel::lib::u32, kernel::lib::u64;
using kernel::lib::log::logger;

namespace kernel::cpu {

namespace {

constexpr int MAX_GATES          = 255;
constexpr u16 SEGMENT_SELECTOR   = 0x08;

/// Reload the IDT.
extern "C" void idt_flush(u64 *idtr);

// ISR stubs
extern "C" void __isr_stub3();
extern "C" void __isr_stub10();
extern "C" void __isr_stub13();
extern "C" void __isr_stub14();

// IRQ stubs
extern "C" void __irq_stub0();
extern "C" void __irq_stub1();

// Syscall
extern "C" void syscall_common();

/// Easy-to-use representation of a single IDT entry.
struct [[gnu::packed]] IDTEntry {
        u16 isr_low;
        u16 selector;
        u8  ist;        // 3 bits for the IST and the 5 other bits are reserved
        u8  flags;      // gate type + 0 + dpl + p
        u16 isr_mid;
        u32 isr_high;
        u32 __reserved;
};

/// Easy-to-use representation of the IDT register.
struct [[gnu::packed]] IDTR {
        u16 size;
        u64 offset;
};

IDTEntry idt[MAX_GATES];
IDTR idtptr;

} /* anonymous namespace */

// --------------------------------------------------
IDT::IDT()
{
        idtptr = {
                .size = sizeof(idt) - 1,
                .offset = reinterpret_cast<u64>(&idt)
        };

        // isr
        this->set_gate(3, __isr_stub3, 0x8e);
        this->set_gate(10, __isr_stub10, 0x8e);
        this->set_gate(13, __isr_stub13, 0x8e);
        this->set_gate(14, __isr_stub14, 0x8e);
        // irq
        this->set_gate(32, __irq_stub0, 0x8e);
        this->set_gate(33, __irq_stub1, 0x8e);
        // syscall
        this->set_gate(128, syscall_common, 0xef);

        logger.ok("initialized idt");
}
// --------------------------------------------------

// --------------------------------------------------
void IDT::load()
{
        idt_flush(reinterpret_cast<u64 *>(&idtptr));

        logger.ok("loaded idt");
}
// --------------------------------------------------

// --------------------------------------------------
void IDT::set_gate(int vector, void (*isr)(), u8 flags)
{
        idt[vector].isr_low     = reinterpret_cast<u64>(isr) & 0xffff;
        idt[vector].selector    = SEGMENT_SELECTOR;
        idt[vector].ist         = 0;
        idt[vector].flags       = flags;
        idt[vector].isr_mid     = (reinterpret_cast<u64>(isr) >> 16) & 0xffff;
        idt[vector].isr_high    = (reinterpret_cast<u64>(isr) >> 32) & 0xffffffff;
        idt[vector].__reserved  = 0;
}
// --------------------------------------------------

} /* namespace kernel::cpu */