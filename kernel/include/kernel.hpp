#pragma once

#include <cpu/idt.hpp>
#include <mem/vmm.hpp>
#include <lib/typing.hpp>

namespace kernel {

constexpr lib::uptr KERNEL_STACK_TOP = 0xfffffffffffff000;
constexpr lib::uptr KERNEL_STACK_SIZE = 64 * mem::vmm::PAGE_BYTES;
constexpr lib::uptr KERNEL_STACK_BOTTOM = KERNEL_STACK_TOP - KERNEL_STACK_SIZE;

/// Return the PML4 table used by the kernel.
lib::u64 *get_kernel_pml4t();
/// Set the PML4 table that the kernel must use.
void set_kernel_pml4t(lib::u64 *pml4t);
/// Return the higher-half direct mapping offset.
lib::u64 get_kernel_hhdm_offset();
/// Set the higher-half direct mapping offset.
void set_kernel_hhdm_offset(lib::u64 offset);
cpu::IDT &get_kernel_idt();
void set_kernel_idt(const cpu::IDT &idt);

} /* namespace kernel */