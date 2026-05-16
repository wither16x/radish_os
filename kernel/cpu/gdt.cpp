#include <cpu/gdt.hpp>
#include <lib/logging.hpp>

using namespace kernel::lib;

namespace kernel::cpu {

namespace {

extern "C" void gdt_flush(u64 gdtr);

constexpr int MaxDescriptors = 3;

struct [[gnu::packed]] gdt_descriptor {
        u16 limit_low;
        u16 base_low;
        u8  base_middle;
        u8  access;
        u8  limit_and_flags;
        u8  base_high;
};

struct [[gnu::packed]] gdtr {
        u16 size;
        u64 offset;
};

gdt_descriptor gdt[MaxDescriptors];
gdtr gdtptr;

} /* anonymous namespace */

GDT::GDT()
{
        log::status("initializing gdt");

        gdtptr = {
                .size = sizeof(gdt) * MaxDescriptors - 1,
                .offset = reinterpret_cast<u64>(&gdt)
        };

        this->set_descriptor(0, 0, 0, 0, 0);
        this->set_descriptor(1, 0, 0, 0x9a, 0xa0);
        this->set_descriptor(2, 0, 0, 0x92, 0);

        log::ok();
}

void GDT::load()
{
        log::status("loading gdt");

        gdt_flush(reinterpret_cast<u64>(&gdtptr));

        log::ok();
}

void GDT::set_descriptor(int n, u32 base, u32 limit, u8 access, u8 flags)
{
        gdt[n].limit_low        = limit & 0xffff;
        gdt[n].base_low         = base & 0xffff;
        gdt[n].base_middle      = (base >> 16) & 0xff;
        gdt[n].access           = access;
        gdt[n].limit_and_flags  = flags & 0xf0;
        gdt[n].base_high        = base >> 16;
}

} /* namespace kernel::cpu */