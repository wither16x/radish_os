#include <cpu/idt.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>

#define ISR(n)                  extern "C" void __isr_stub##n()
#define IRQ(n)                  extern "C" void __irq_stub##n()

namespace Kiwi::Cpu
{
        namespace
        {
                /// Reload the IDT.
                extern "C" void __idt_flush(Lib::u64 *idtr);

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
        } // anonymous namespace

        void Idt::init(this Idt &self)
        {
                self.idtptr = {
                        .size = sizeof(self.gates) - 1,
                        .offset = reinterpret_cast<Lib::u64>(&self.gates)
                };

                // isr
                self.setGate(3, __isr_stub3, 0x8e);
                self.setGate(10, __isr_stub10, 0x8e);
                self.setGate(13, __isr_stub13, 0x8e);
                self.setGate(14, __isr_stub14, 0x8e);
                // irq
                self.setGate(32, __irq_stub0, 0x8e);
                self.setGate(33, __irq_stub1, 0x8e);
                // syscall
                self.setGate(128, syscall_common, 0xef);

                Lib::Log::logger.ok("initialized idt");
        }

        void Idt::load(this Idt &self)
        {
                __idt_flush(reinterpret_cast<Lib::u64 *>(&self.idtptr));

                Lib::Log::logger.ok("loaded idt");
        }

        void Idt::setGate(this Idt &self, int vector, void (*isr)(), Lib::u8 flags)
        {
                self.gates[vector].isr_low     = reinterpret_cast<Lib::u64>(isr) & 0xffff;
                self.gates[vector].selector    = SEGMENT_SELECTOR;
                self.gates[vector].ist         = 0;
                self.gates[vector].flags       = flags;
                self.gates[vector].isr_mid     = (reinterpret_cast<Lib::u64>(isr) >> 16) & 0xffff;
                self.gates[vector].isr_high    = (reinterpret_cast<Lib::u64>(isr) >> 32) & 0xffffffff;
                self.gates[vector].__reserved  = 0;
        }
} // namespace Kiwi::Cpu