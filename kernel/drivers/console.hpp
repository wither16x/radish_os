#pragma once

#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>

namespace kernel::drivers::console {

class Console {
public:
        void init_font(const lib::String &font);
        void draw_char(char ch, int x, int y);

private:
        int cursor_x;
        int cursor_y;

        lib::Vector<lib::u8> font_data;

        lib::u32 glyph_offset;
        lib::u32 glyph_size;
        lib::u32 glyph_width;
        lib::u32 glyph_height;
};

void set_console(Console console);
Console *get_console();

} /* namespace kernel::drivers::console */