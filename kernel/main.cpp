#include <limine.h>

namespace {

[[gnu::used, gnu::section(".limine_requests")]]
volatile unsigned long long limine_base_revision[] = LIMINE_BASE_REVISION(6);

}

namespace {

[[gnu::used, gnu::section(".limine_requests_start")]]
volatile unsigned long long limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

[[gnu::used, gnu::section(".limine_requests_end")]]
volatile unsigned long long limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

}

extern void (*__init_array[])();
extern void (*__init_array_end[])();

extern "C" void kernel_main()
{
        if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
                while (true)
                        __asm__("hlt");
        }

        for (unsigned long long i = 0; &__init_array[i] != __init_array_end; i++)
                __init_array[i]();

        while (true)
                __asm__("hlt");
}