#include <limine.h>

namespace {

[[gnu::used, gnu::section(".limine_requests")]]
volatile unsigned long long limine_base_revision[] = LIMINE_BASE_REVISION(6);

}

namespace {

[[gnu::used, gnu::section(".limine_requests")]]
volatile limine_framebuffer_request framebuffer_request = {
        .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
        .revision = 0,
        .response = nullptr
};

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

        if (framebuffer_request.response == nullptr || framebuffer_request.response->framebuffer_count < 1) {
                while (true)
                        __asm__("hlt");
        }

        limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

        volatile unsigned int *fb_ptr = static_cast<volatile unsigned int *>(framebuffer->address);
        for (unsigned long long y = 0; y < framebuffer->height; y++) {
                for (unsigned long long x = 0; x < framebuffer->width; x++) {
                        unsigned int nX = x * 255 / framebuffer->width;
                        unsigned int nY = y * 255 / framebuffer->height;
                        fb_ptr[y * (framebuffer->pitch / 4) + x] = (nY << 8) | nX;
                }
        }

        while (true)
                __asm__("hlt");
}