#include "lib/memory.hpp"
#include <lib/typing.hpp>
#include <cpu/gdt.hpp>
#include <cpu/tss.hpp>
#include <kernel.hpp>
#include <lib/logging.hpp>

using kernel::lib::u8, kernel::lib::u16, kernel::lib::u32, kernel::lib::u64, kernel::lib::uptr;
using kernel::lib::log::logger;

namespace kernel::cpu {

namespace {

/// Reload the GDT.
extern "C" void gdt_flush(u64 gdtr);

constexpr int MAX_DESCRIPTORS = 7;

/// Easy-to-use representation of a single GDT descriptor.
struct [[gnu::packed]] GDTDescriptor {
        u16 limit_low;
        u16 base_low;
        u8  base_middle;
        u8  access;
        u8  limit_and_flags;
        u8  base_high;
};

/// Easy-to-use representation of the GDT register.
struct [[gnu::packed]] GDTR {
        u16 size;
        u64 offset;
};

GDTDescriptor gdt[MAX_DESCRIPTORS];
GDTR gdtptr;

TSS tss;

} /* anonymous namespace */

// --------------------------------------------------
GDT::GDT()
{
        gdtptr = {
                .size = sizeof(gdt) - 1,
                .offset = reinterpret_cast<u64>(&gdt)
        };

        init_tss(&tss, reinterpret_cast<uptr>(KERNEL_STACK_TOP - 8));

        this->set_descriptor(0, 0, 0, 0, 0); // null
        this->set_descriptor(1, 0, 0, 0x9a, 0xa0); // kernel code
        this->set_descriptor(2, 0, 0, 0x92, 0); // kernel data
        this->set_descriptor(3, 0, 0, 0xfa, 0xa0); // user code
        this->set_descriptor(4, 0, 0, 0xf2, 0xc); // user data
        // in long mode the TSS takes two entries
        this->set_descriptor(5, reinterpret_cast<u64>(&tss) & 0xffffffff, sizeof(tss) - 1, 0x89, 0);
        lib::memset(&gdt[6], 0, sizeof(GDTDescriptor));
        *reinterpret_cast<u32 *>(&gdt[6]) = (reinterpret_cast<u64>(&tss) >> 32) & 0xffffffff;

        logger.ok("initialized gdt");
}
// --------------------------------------------------

// --------------------------------------------------
void GDT::load()
{
        gdt_flush(reinterpret_cast<u64>(&gdtptr));

        logger.ok("loaded gdt");
}
// --------------------------------------------------

// --------------------------------------------------
void GDT::set_descriptor(int n, u32 base, u32 limit, u8 access, u8 flags)
{
        gdt[n].limit_low        = limit & 0xffff;
        gdt[n].base_low         = base & 0xffff;
        gdt[n].base_middle      = (base >> 16) & 0xff;
        gdt[n].access           = access;
        gdt[n].limit_and_flags  = (flags & 0xf0) | ((limit >> 16) & 0x0f);
        gdt[n].base_high        = base >> 24;
}
// --------------------------------------------------

} /* namespace kernel::cpu */