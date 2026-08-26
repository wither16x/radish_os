#pragma once

#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>

namespace Kiwi::Drivers::Console
{
        /// Graphical (framebuffer-based) console.
        class Console
        {
                Lib::u64 cursor_x;
                Lib::u64 cursor_y;

                // both values below are in pixels
                Lib::u64 width;
                Lib::u64 height;

                Lib::Vector<Lib::u8> font_data;

                Lib::u32 glyph_offset;
                Lib::u32 glyph_size;
                Lib::u32 glyph_width;
                Lib::u32 glyph_height;

                bool active = false;

                /// Draw a character at a given position.
                void drawCharAt(this Console &self, char ch, int px, int py, Lib::u32 color);
                /// Scroll the text up.
                void scroll(this Console &self);

        public:
                Console() = default;
                Console(Lib::u64 width, Lib::u64 height);

                ~Console();

                /// Initialize the font used by the console (see implementation
                /// for more details.)
                void initFont(this Console &self, const Lib::String &font);
                /// Draw a character at the current cursor position.
                void drawChar(this Console &self, char ch, Lib::u32 color);

                /// Check if the console is active or not. If a `Console`
                /// is active, then it can be used safely.
                bool isActive(this const Console &self);
                /// Get the data of the console font as a vector.
                const Lib::Vector<Lib::u8> &getFontData(this const Console &self);
        };

        /// Set the currently used console.
        void setConsole(const Console &console);
        /// Get the currently used console.
        Console &getConsole();
} // namespace Kiwi::Drivers::Console