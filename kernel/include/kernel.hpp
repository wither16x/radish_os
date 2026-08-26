#pragma once

#include <cpu/gdt.hpp>
#include <cpu/idt.hpp>
#include <mem/pml4t.hpp>
#include <mem/page.hpp>
#include <lib/typing.hpp>

namespace Kiwi
{
        constexpr Lib::uptr KERNEL_STACK_TOP = 0xfffffffffffff000;
        constexpr Lib::uptr KERNEL_STACK_SIZE = 64 * Mem::PAGE_SIZE;
        constexpr Lib::uptr KERNEL_STACK_BOTTOM = KERNEL_STACK_TOP - KERNEL_STACK_SIZE;

        /// Return the PML4 table used by the kernel.
        Mem::PML4T &getKernelPml4t();
        /// Set the PML4 table that the kernel must use.
        void setKernelPml4t(Mem::PML4T &pml4t);

        /// Return the higher-half direct mapping offset.
        Lib::u64 getKernelHhdmOffset();
        /// Set the higher-half direct mapping offset.
        void setKernelHhdmOffset(Lib::u64 offset);

        Cpu::Idt &getKernelIdt();
        void setKernelIdt(const Cpu::Idt &idt);

        Cpu::Gdt &get_kernel_gdt();
        void set_kernel_gdt(const Cpu::Gdt &gdt);
} // namespace Kiwi