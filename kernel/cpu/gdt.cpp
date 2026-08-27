#include <lib/memory.hpp>
#include <lib/typing.hpp>
#include <cpu/gdt.hpp>
#include <kernel.hpp>
#include <lib/logging.hpp>

namespace Kiwi::Cpu
{
        namespace
        {
                /// Reload the Gdt.
                extern "C" void __gdt_flush(Lib::u64 Gdtr);
        } // anonymous namespace

        void Gdt::init(this Gdt &self)
        {
                self.gdtptr = {
                        .size = sizeof(self.descriptors) - 1,
                        .offset = reinterpret_cast<Lib::u64>(&self.descriptors)
                };

                self.tss.init(KERNEL_STACK_TOP - 8);

                self.setDescriptor(0, 0, 0, 0, 0); // null
                self.setDescriptor(1, 0, 0, 0x9a, 0xa0); // kernel code
                self.setDescriptor(2, 0, 0, 0x92, 0); // kernel data
                self.setDescriptor(3, 0, 0, 0xfa, 0xa0); // user code
                self.setDescriptor(4, 0, 0, 0xf2, 0xc); // user data
                // in long mode the TSS takes two entries
                self.setDescriptor(5, reinterpret_cast<Lib::u64>(&self.tss.getData()) & 0xffffffff, sizeof(TssData) - 1, 0x89, 0);
                Lib::memset(&self.descriptors[6], 0, sizeof(GdtDescriptor));
                *reinterpret_cast<Lib::u32 *>(&self.descriptors[6]) = (reinterpret_cast<Lib::u64>(&self.tss.getData()) >> 32) & 0xffffffff;

                Lib::Log::logger.ok("initialized gdt");
        }

        void Gdt::load(this Gdt &self)
        {
                __gdt_flush(reinterpret_cast<Lib::u64>(&self.gdtptr));

                Lib::Log::logger.ok("loaded gdt");
        }

        void Gdt::setDescriptor(this Gdt &self, int n, Lib::u32 base, Lib::u32 limit, Lib::u8 access, Lib::u8 flags)
        {
                self.descriptors[n].limit_low        = limit & 0xffff;
                self.descriptors[n].base_low         = base & 0xffff;
                self.descriptors[n].base_middle      = (base >> 16) & 0xff;
                self.descriptors[n].access           = access;
                self.descriptors[n].limit_and_flags  = (flags & 0xf0) | ((limit >> 16) & 0x0f);
                self.descriptors[n].base_high        = base >> 24;
        }

        Tss &Gdt::getTss(this Gdt &self)
        {
                return self.tss;
        }
} // namespace Kiwi::Cpu