#include <cpu/idt.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>

using namespace kernel::lib;

namespace kernel::cpu {

namespace {

constexpr int MaxGates          = 255;
constexpr u16 SegmentSelector   = 0x08;

extern "C" void idt_flush(u64 *idtr);

extern "C" void __isr_stub3();
extern "C" void __isr_stub14();

struct [[gnu::packed]] idt_entry {
        u16 isr_low;
        u16 selector;
        u8  ist;        // 3 bits for the IST and the 5 other bits are reserved
        u8  flags;      // gate type + 0 + dpl + p
        u16 isr_mid;
        u32 isr_high;
        u32 __reserved;
};

struct [[gnu::packed]] idtr {
        u16 size;
        u64 offset;
};

idt_entry idt[MaxGates];
idtr idtptr;

} /* anonymous namespace */

IDT::IDT()
{
        log::status("initializing idt");

        idtptr = {
                .size = sizeof(idt) - 1,
                .offset = reinterpret_cast<u64>(&idt)
        };

        this->set_gate(3, __isr_stub3, 0x8e);
        this->set_gate(14, __isr_stub14, 0x8e);

        log::ok();
}

void IDT::load() const
{
        log::status("loading idt");

        idt_flush(reinterpret_cast<u64 *>(&idtptr));

        log::ok();
}

void IDT::set_gate(int vector, void (*isr)(), lib::u8 flags) const
{
        idt[vector].isr_low     = reinterpret_cast<u64>(isr) & 0xffff;
        idt[vector].selector    = SegmentSelector;
        idt[vector].ist         = 0;
        idt[vector].flags       = flags;
        idt[vector].isr_mid     = (reinterpret_cast<u64>(isr) >> 16) & 0xffff;
        idt[vector].isr_high    = (reinterpret_cast<u64>(isr) >> 32) & 0xffffffff;
        idt[vector].__reserved  = 0;
}

} /* namespace kernel::cpu */