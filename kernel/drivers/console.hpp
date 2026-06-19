#pragma once

#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>

namespace kernel::drivers::console {

class Console {
public:
        Console() = default;
        Console(lib::u64 width, lib::u64 height);

        ~Console();

        void init_font(this Console &self, const lib::String &font);
        void draw_char(this Console &self, char ch, lib::u32 color);

        bool is_active(this const Console &self);

private:
        lib::u64 cursor_x;
        lib::u64 cursor_y;

        // both values below are in pixels
        lib::u64 width;
        lib::u64 height;

        lib::Vector<lib::u8> font_data;

        lib::u32 glyph_offset;
        lib::u32 glyph_size;
        lib::u32 glyph_width;
        lib::u32 glyph_height;

        bool active = false;

        void draw_char_at(this Console &self, char ch, int px, int py, lib::u32 color);
        void scroll(this Console &self);
};

void set_console(Console console);
Console *get_console();

} /* namespace kernel::drivers::console */