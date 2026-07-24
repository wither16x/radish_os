#include <mem/pml4t.hpp>
#include <cpu/gdt.hpp>
#include <cpu/idt.hpp>
#include <kernel.hpp>
#include <lib/typing.hpp>

using kernel::lib::u8, kernel::lib::u64;

namespace kernel {

namespace {

mem::PML4T kpml4t;
u64 hhdm_offset;
cpu::IDT kidt;
cpu::GDT kgdt;

} /* anonymous namespace */

// --------------------------------------------------
mem::PML4T &get_kernel_pml4t()
{
        return kpml4t;
}
// --------------------------------------------------

// --------------------------------------------------
void set_kernel_pml4t(mem::PML4T &pml4t)
{
        kpml4t = pml4t;
}
// --------------------------------------------------

// --------------------------------------------------
u64 get_kernel_hhdm_offset()
{
        return hhdm_offset;
}
// --------------------------------------------------

// --------------------------------------------------
void set_kernel_hhdm_offset(lib::u64 offset)
{
        hhdm_offset = offset;
}
// --------------------------------------------------

// --------------------------------------------------
cpu::IDT &get_kernel_idt()
{
        return kidt;
}
// --------------------------------------------------

// --------------------------------------------------
void set_kernel_idt(const cpu::IDT &idt)
{
        kidt = idt;
}
// --------------------------------------------------

cpu::GDT &get_kernel_gdt()
{
        return kgdt;
}

void set_kernel_gdt(const cpu::GDT &gdt)
{
        kgdt = gdt;
}

} /* namespace kernel */