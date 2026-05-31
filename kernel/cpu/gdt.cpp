#include <cpu/gdt.hpp>
#include <lib/logging.hpp>

using kernel::lib::u8, kernel::lib::u16, kernel::lib::u32, kernel::lib::u64;
using kernel::lib::log::logger;

namespace kernel::cpu {

namespace {

extern "C" void gdt_flush(u64 gdtr);

constexpr int MaxDescriptors = 3;

struct [[gnu::packed]] GDTDescriptor {
        u16 limit_low;
        u16 base_low;
        u8  base_middle;
        u8  access;
        u8  limit_and_flags;
        u8  base_high;
};

struct [[gnu::packed]] GDTR {
        u16 size;
        u64 offset;
};

GDTDescriptor gdt[MaxDescriptors];
GDTR gdtptr;

} /* anonymous namespace */

GDT::GDT()
{
        logger.info("initializing gdt...");

        gdtptr = {
                .size = sizeof(gdt) - 1,
                .offset = reinterpret_cast<u64>(&gdt)
        };

        this->set_descriptor(0, 0, 0, 0, 0);
        this->set_descriptor(1, 0, 0, 0x9a, 0xa0);
        this->set_descriptor(2, 0, 0, 0x92, 0);

        logger.ok("initialized gdt");
}

void GDT::load()
{
        logger.info("loading gdt...");

        gdt_flush(reinterpret_cast<u64>(&gdtptr));

        logger.ok("loaded gdt");
}

void GDT::set_descriptor(int n, u32 base, u32 limit, u8 access, u8 flags) const
{
        gdt[n].limit_low        = limit & 0xffff;
        gdt[n].base_low         = base & 0xffff;
        gdt[n].base_middle      = (base >> 16) & 0xff;
        gdt[n].access           = access;
        gdt[n].limit_and_flags  = flags & 0xf0;
        gdt[n].base_high        = base >> 16;
}

} /* namespace kernel::cpu */