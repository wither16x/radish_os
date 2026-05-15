#include <boot/limine.hpp>
#include <lib/typing.hpp>

using namespace kernel;

namespace {

[[gnu::used, gnu::section(".limine_requests")]]
volatile limine::revision limine_base_revision = limine::base_revision(6);

}

namespace {

[[gnu::used, gnu::section(".limine_requests_start")]]
volatile limine::start_marker limine_requests_start_marker = limine::requests_start_marker();

[[gnu::used, gnu::section(".limine_requests_end")]]
volatile limine::end_marker limine_requests_end_marker = limine::requests_end_marker();

}

extern void (*__init_array[])();
extern void (*__init_array_end[])();

extern "C" void kernel_main()
{
        if (limine_base_revision.is_supported()) {
                while (true)
                        __asm__("hlt");
        }

        for (u64 i = 0; &__init_array[i] != __init_array_end; i++)
                __init_array[i]();

        while (true)
                __asm__("hlt");
}