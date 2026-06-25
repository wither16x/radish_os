#pragma once

#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>

namespace kernel::drivers::console {

/// Graphical (framebuffer-based) console.
class Console {
public:
        Console() = default;
        Console(lib::u64 width, lib::u64 height);

        ~Console();

        /// Initialize the font used by the console (see implementation
        /// for more details.)
        void init_font(this Console &self, const lib::String &font);
        /// Draw a character at the current cursor position.
        void draw_char(this Console &self, char ch, lib::u32 color);

        /// Check if the console is active or not. If a `Console`
        /// is active, then it can be used safely.
        bool is_active(this const Console &self);
        /// Get the data of the console font as a vector.
        const lib::Vector<lib::u8> &get_font_data(this const Console &self);

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

        /// Draw a character at a given position.
        void draw_char_at(this Console &self, char ch, int px, int py, lib::u32 color);
        /// Scroll the text up.
        void scroll(this Console &self);
};

/// Set the currently used console.
void set_console(const Console &console);
/// Get the currently used console.
Console &get_console();

} /* namespace kernel::drivers::console */