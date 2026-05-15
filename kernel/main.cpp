#include <boot/limine.hpp>
#include <cpu/assembly.hpp>
#include <lib/typing.hpp>

using namespace kernel;
using namespace kernel::typing;

namespace {

[[gnu::used, gnu::section(".limine_requests")]]
volatile boot::limine::revision limine_base_revision = boot::limine::base_revision(6);

}

namespace {

[[gnu::used, gnu::section(".limine_requests_start")]]
volatile boot::limine::start_marker limine_requests_start_marker = boot::limine::requests_start_marker();

[[gnu::used, gnu::section(".limine_requests_end")]]
volatile boot::limine::end_marker limine_requests_end_marker = boot::limine::requests_end_marker();

}

extern void (*__init_array[])();
extern void (*__init_array_end[])();

extern "C" void kernel_main()
{
        if (limine_base_revision.is_supported()) {
                while (true)
                        cpu::assembly::hlt();
        }

        for (u64 i = 0; &__init_array[i] != __init_array_end; i++)
                __init_array[i]();

        while (true)
                cpu::assembly::hlt();
}