#include <mem/pml4t.hpp>
#include <cpu/gdt.hpp>
#include <cpu/idt.hpp>
#include <kernel.hpp>
#include <lib/typing.hpp>

namespace Kiwi
{
        void KernelContext::setPml4t(this KernelContext &self, const Mem::PML4T &pml4t)
        {
                self._pml4t = pml4t;
        }

        void KernelContext::setHhdm(this KernelContext &self, Lib::u64 hhdm)
        {
                self._hhdm = hhdm;
        }

        void KernelContext::setIdt(this KernelContext &self, const Cpu::Idt &idt)
        {
                self._idt = idt;
        }

        void KernelContext::setGdt(this KernelContext &self, const Cpu::Gdt &gdt)
        {
                self._gdt = gdt;
        }

        Mem::PML4T &KernelContext::pml4t(this KernelContext &self)
        {
                return self._pml4t;
        }

        Lib::u64 KernelContext::hhdm(this const KernelContext &self)
        {
                return self._hhdm;
        }

        Cpu::Idt &KernelContext::idt(this KernelContext &self)
        {
                return self._idt;
        }

        Cpu::Gdt &KernelContext::gdt(this KernelContext &self)
        {
                return self._gdt;
        }
} // namespace Kiwi