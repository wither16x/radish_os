#pragma once

#include <cpu/gdt.hpp>
#include <cpu/idt.hpp>
#include <mem/pml4t.hpp>
#include <mem/page.hpp>
#include <lib/typing.hpp>

namespace kernel {

constexpr lib::uptr KERNEL_STACK_TOP = 0xfffffffffffff000;
constexpr lib::uptr KERNEL_STACK_SIZE = 64 * mem::PAGE_SIZE;
constexpr lib::uptr KERNEL_STACK_BOTTOM = KERNEL_STACK_TOP - KERNEL_STACK_SIZE;

/// Return the PML4 table used by the kernel.
mem::PML4T &get_kernel_pml4t();
/// Set the PML4 table that the kernel must use.
void set_kernel_pml4t(mem::PML4T &pml4t);

/// Return the higher-half direct mapping offset.
lib::u64 get_kernel_hhdm_offset();
/// Set the higher-half direct mapping offset.
void set_kernel_hhdm_offset(lib::u64 offset);

cpu::IDT &get_kernel_idt();
void set_kernel_idt(const cpu::IDT &idt);

cpu::GDT &get_kernel_gdt();
void set_kernel_gdt(const cpu::GDT &gdt);

} /* namespace kernel */