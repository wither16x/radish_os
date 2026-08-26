#pragma once

#include <lib/typing.hpp>

namespace Kiwi::Drivers::Framebuffer
{
        /// Initialize the framebuffer informations.
        void init(void *address, Lib::u64 width, Lib::u64 height, Lib::u64 pitch);
        /// Draw a pixel at a given position in the framebuffer.
        void drawPixel(Lib::u64 x, Lib::u64 y, Lib::u32 color);
        /// Scroll `height` rows up.
        void scroll(Lib::u64 height);
        /// Get the framebuffer width.
        Lib::u64 getWidth();
        /// Get the framebuffer height.
        Lib::u64 getHeight();
        void drawRectangle(Lib::u64 start_x, Lib::u64 start_y, Lib::u64 width, Lib::u64 height, Lib::u32 color);
} // namespace Kiwi::Drivers::Framebuffer