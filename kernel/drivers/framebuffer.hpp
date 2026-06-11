#pragma once

#include <lib/typing.hpp>

namespace kernel::drivers::framebuffer {

void init(void *address, lib::u64 width, lib::u64 height, lib::u64 pitch);
void draw_pixel(lib::u64 x, lib::u64 y, lib::u32 color);

} /* namespace kernel::drivers::framebuffer */