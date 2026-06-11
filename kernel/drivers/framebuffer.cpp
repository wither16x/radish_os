#include <drivers/framebuffer.hpp>
#include <lib/typing.hpp>

using kernel::lib::u32, kernel::lib::u64;

namespace kernel::drivers::framebuffer {

namespace {

u32 *fb_ptr = nullptr;
u64 fb_width = 0;
u64 fb_height = 0;
u64 fb_pitch  = 0;

} /* anonymous namespace */

void init(void *address, u64 width, u64 height, u64 pitch)
{
        fb_ptr = static_cast<u32 *>(address);
        fb_width = width;
        fb_height = height;
        fb_pitch = pitch;
}

void draw_pixel(lib::u64 x, lib::u64 y, lib::u32 color)
{
        if (x < 0 || x > fb_width || y < 0 || y > fb_height)
                return;

        fb_ptr[y * (fb_pitch / 4) + x] = color;
}

} /* namespace kernel::drivers::framebuffer */