#include <mem/pml4t.hpp>
#include <cpu/gdt.hpp>
#include <cpu/idt.hpp>
#include <kernel.hpp>
#include <lib/typing.hpp>

namespace Kiwi
{
        namespace
        {
                Mem::PML4T kpml4t;
                Lib::u64 hhdm_offset;
                Cpu::Idt kidt;
                Cpu::Gdt kgdt;
        } // anonymous namespace

        Mem::PML4T &getKernelPml4t()
        {
                return kpml4t;
        }

        void setKernelPml4t(Mem::PML4T &pml4t)
        {
                kpml4t = pml4t;
        }

        Lib::u64 getKernelHhdmOffset()
        {
                return hhdm_offset;
        }

        void setKernelHhdmOffset(Lib::u64 offset)
        {
                hhdm_offset = offset;
        }

        Cpu::Idt &getKernelIdt()
        {
                return kidt;
        }

        void setKernelIdt(const Cpu::Idt &idt)
        {
                kidt = idt;
        }

        Cpu::Gdt &getKernelGdt()
        {
                return kgdt;
        }

        void setKernelGdt(const Cpu::Gdt &gdt)
        {
                kgdt = gdt;
        }
} // namespace Kiwi