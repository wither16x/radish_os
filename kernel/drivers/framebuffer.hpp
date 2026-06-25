#pragma once

#include <lib/typing.hpp>

namespace kernel::drivers::framebuffer {

/// Initialize the framebuffer informations.
void init(void *address, lib::u64 width, lib::u64 height, lib::u64 pitch);
/// Draw a pixel at a given position in the framebuffer.
void draw_pixel(lib::u64 x, lib::u64 y, lib::u32 color);
/// Scroll `height` rows up.
void scroll(lib::u64 height);
/// Get the framebuffer width.
lib::u64 get_width();
/// Get the framebuffer height.
lib::u64 get_height();

} /* namespace kernel::drivers::framebuffer */