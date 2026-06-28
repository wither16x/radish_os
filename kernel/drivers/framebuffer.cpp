#include <drivers/framebuffer.hpp>
#include <lib/memory.hpp>
#include <lib/typing.hpp>

using kernel::lib::u8, kernel::lib::u32, kernel::lib::u64, kernel::lib::uptr;
using kernel::lib::memcpy, kernel::lib::memset;

namespace kernel::drivers::framebuffer {

namespace {

u32 *fb_ptr = nullptr;
u64 fb_width = 0;
u64 fb_height = 0;
u64 fb_pitch  = 0;

} /* anonymous namespace */

// --------------------------------------------------
void init(void *address, u64 width, u64 height, u64 pitch)
{
        fb_ptr = static_cast<u32 *>(address);
        fb_width = width;
        fb_height = height;
        fb_pitch = pitch;
}
// --------------------------------------------------

// --------------------------------------------------
void draw_pixel(lib::u64 x, lib::u64 y, lib::u32 color)
{
        if (x < 0 || x > fb_width || y < 0 || y > fb_height)
                return;

        fb_ptr[y * (fb_pitch / 4) + x] = color;
}
// --------------------------------------------------

// --------------------------------------------------
void scroll(u64 height)
{
        u8 *base = reinterpret_cast<u8 *>(fb_ptr);

        if (height >= fb_height) {
                memset(base, 0, fb_pitch * fb_height);
                return;
        }

        for (u64 y = height; y != fb_height; ++y) {
                void *dest = base + (y - height) * fb_pitch;
                const void *src = base + y * fb_pitch; 

                memcpy(dest, src, fb_width * 4);
        }

        for (u64 y = fb_height - height; y != fb_height; ++y) {
                u32 *dest = reinterpret_cast<u32 *>(base + y *fb_pitch);

                for (u64 i = 0; i != fb_width; ++i)
                        *dest++ = 0x000000;
        }
}
// --------------------------------------------------

// --------------------------------------------------
u64 get_width()
{
        return fb_width;
}
// --------------------------------------------------

// --------------------------------------------------
u64 get_height()
{
        return fb_height;
}
// --------------------------------------------------

// --------------------------------------------------
void draw_rectangle(u64 start_x, u64 start_y, u64 width, u64 height, u32 color)
{
        for (u64 y = start_y; y < start_y + height; y++) {
                for (u64 x = start_x; x < start_x + width; x++)
                        draw_pixel(x, y, color);
        }
}
// --------------------------------------------------

} /* namespace kernel::drivers::framebuffer */