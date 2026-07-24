#include <lib/memory.hpp>
#include <lib/typing.hpp>
#include <cpu/gdt.hpp>
#include <kernel.hpp>
#include <lib/logging.hpp>

using kernel::lib::u8, kernel::lib::u16, kernel::lib::u32, kernel::lib::u64, kernel::lib::uptr;
using kernel::lib::log::logger;

namespace kernel::cpu {

namespace {

/// Reload the GDT.
extern "C" void __gdt_flush(u64 gdtr);

} /* anonymous namespace */

// --------------------------------------------------
void GDT::init(this GDT &self)
{
        self.gdtptr = {
                .size = sizeof(self.descriptors) - 1,
                .offset = reinterpret_cast<u64>(&self.descriptors)
        };

        self.tss.init(KERNEL_STACK_TOP - 8);

        self.set_descriptor(0, 0, 0, 0, 0); // null
        self.set_descriptor(1, 0, 0, 0x9a, 0xa0); // kernel code
        self.set_descriptor(2, 0, 0, 0x92, 0); // kernel data
        self.set_descriptor(3, 0, 0, 0xfa, 0xa0); // user code
        self.set_descriptor(4, 0, 0, 0xf2, 0xc); // user data
        // in long mode the TSS takes two entries
        self.set_descriptor(5, reinterpret_cast<u64>(&self.tss.get_data()) & 0xffffffff, sizeof(TSSData) - 1, 0x89, 0);
        lib::memset(&self.descriptors[6], 0, sizeof(GDTDescriptor));
        *reinterpret_cast<u32 *>(&self.descriptors[6]) = (reinterpret_cast<u64>(&self.tss.get_data()) >> 32) & 0xffffffff;

        logger.ok("initialized gdt");
}
// --------------------------------------------------

// --------------------------------------------------
void GDT::load()
{
        __gdt_flush(reinterpret_cast<u64>(&gdtptr));

        logger.ok("loaded gdt");
}
// --------------------------------------------------

// --------------------------------------------------
void GDT::set_descriptor(this GDT &self, int n, u32 base, u32 limit, u8 access, u8 flags)
{
        self.descriptors[n].limit_low        = limit & 0xffff;
        self.descriptors[n].base_low         = base & 0xffff;
        self.descriptors[n].base_middle      = (base >> 16) & 0xff;
        self.descriptors[n].access           = access;
        self.descriptors[n].limit_and_flags  = (flags & 0xf0) | ((limit >> 16) & 0x0f);
        self.descriptors[n].base_high        = base >> 24;
}
// --------------------------------------------------

TSS &GDT::get_tss(this GDT &self)
{
        return self.tss;
}

} /* namespace kernel::cpu */