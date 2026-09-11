#pragma once

#include <cpu/gdt.hpp>
#include <cpu/idt.hpp>
#include <mem/pml4t.hpp>
#include <mem/page.hpp>
#include <lib/typing.hpp>

namespace Kiwi
{
        class KernelContext
        {
                Mem::PML4T _pml4t;
                Lib::u64 _hhdm;
                Cpu::Idt _idt;
                Cpu::Gdt _gdt;

        public:
                static constexpr Lib::uptr STACK_TOP = 0xfffffffffffff000;
                static constexpr Lib::uptr STACK_SIZE = 64 * Mem::PAGE_SIZE;
                static constexpr Lib::uptr STACK_BOTTOM = STACK_TOP - STACK_SIZE;

                void setPml4t(this KernelContext &self, const Mem::PML4T &pml4t);
                void setHhdm(this KernelContext &self, Lib::u64 hhdm);
                void setIdt(this KernelContext &self, const Cpu::Idt &idt);
                void setGdt(this KernelContext &self, const Cpu::Gdt &gdt);

                Mem::PML4T &pml4t(this KernelContext &self);
                Lib::u64 hhdm(this const KernelContext &self);
                Cpu::Idt &idt(this KernelContext &self);
                Cpu::Gdt &gdt(this KernelContext &self);
        };

        inline KernelContext kcontext;
} // namespace Kiwi