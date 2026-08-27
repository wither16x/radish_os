#include <drivers/framebuffer.hpp>
#include <lib/memory.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Drivers::Framebuffer
{
        namespace
        {
                Lib::u32 *fb_ptr = nullptr;
                Lib::u64 fb_width = 0;
                Lib::u64 fb_height = 0;
                Lib::u64 fb_pitch  = 0;
        } // anonymous namespace

        void init(void *address, Lib::u64 width, Lib::u64 height, Lib::u64 pitch)
        {
                fb_ptr = static_cast<Lib::u32 *>(address);
                fb_width = width;
                fb_height = height;
                fb_pitch = pitch;
        }

        void drawPixel(Lib::u64 x, Lib::u64 y, Lib::u32 color)
        {
                if (x < 0 || x > fb_width || y < 0 || y > fb_height)
                        return;

                fb_ptr[y * (fb_pitch / 4) + x] = color;
        }

        void scroll(Lib::u64 height)
        {
                Lib::u8 *base = reinterpret_cast<Lib::u8 *>(fb_ptr);

                if (height >= fb_height) {
                        Lib::memset(base, 0, fb_pitch * fb_height);
                        return;
                }

                for (Lib::u64 y = height; y != fb_height; ++y) {
                        void *dest = base + (y - height) * fb_pitch;
                        const void *src = base + y * fb_pitch; 

                        Lib::memcpy(dest, src, fb_width * 4);
                }

                for (Lib::u64 y = fb_height - height; y != fb_height; ++y) {
                        Lib::u32 *dest = reinterpret_cast<Lib::u32 *>(base + y *fb_pitch);

                        for (Lib::u64 i = 0; i != fb_width; ++i)
                                *dest++ = 0;
                }
        }

        Lib::u64 getWidth()
        {
                return fb_width;
        }

        Lib::u64 getHeight()
        {
                return fb_height;
        }

        void drawRectangle(Lib::u64 start_x, Lib::u64 start_y, Lib::u64 width, Lib::u64 height, Lib::u32 color)
        {
                for (Lib::u64 y = start_y; y < start_y + height; y++) {
                        for (Lib::u64 x = start_x; x < start_x + width; x++)
                                drawPixel(x, y, color);
                }
        }
} // namespace Kiwi::Drivers::Framebuffer