#include <drivers/console.hpp>
#include <drivers/framebuffer.hpp>
#include <lib/filesystem.hpp>
#include <lib/logging.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>

using kernel::lib::String;
using kernel::lib::read_file, kernel::lib::get_file_size;
using kernel::lib::u8, kernel::lib::u32, kernel::lib::u64, kernel::lib::usize;

using kernel::lib::log::logger;

namespace kernel::drivers::console {

namespace {

constexpr u32 PSF2Magic = 0x864ab572;

struct PSF2Header {
        u32 magic;
        u32 version;            // currently always 0 (source: wikipedia)
        u32 hdrsize;
        u32 flags;
        u32 length;             // number of glyphs
        u32 glyph_size;         // bytes per glyph
        u32 height;             // glyph height
        u32 width;              // glyph width
};

Console current_console;

} /* anonymous namespace */

// Console methods
// -------------------------------------------------------------------------

Console::Console(u64 width, u64 height)
{
        this->active    = true;
        this->width     = width;
        this->height    = height;
}

Console::~Console()
{
        this->active = false;
}

void Console::init_font(this Console &self, const String &font)
{
        logger.set_context("console");

        self.cursor_x = 0;
        self.cursor_y = 0;

        usize filesz = get_file_size(font);
        self.font_data.resize(filesz);
        read_file(font, reinterpret_cast<char *>(self.font_data.get_data()), filesz);

        PSF2Header *hdr = reinterpret_cast<PSF2Header *>(self.font_data.get_data());

        if (hdr->magic != PSF2Magic) {
                logger.err("invalid PSF2 magic dword: 0x%x", hdr->magic);
                logger.set_context("kernel");
                return;
        }

        self.glyph_offset       = hdr->hdrsize;
        self.glyph_size         = hdr->glyph_size;
        self.glyph_width        = hdr->width;
        self.glyph_height       = hdr->height;

        logger.set_context("kernel");
}

void Console::draw_char(this Console &self, char ch)
{
        switch (ch) {
        case '\r':
                self.cursor_x = 0;
                break;

        case '\n':
                self.cursor_y += self.glyph_height;
                break;

        case '\b':
                if (self.cursor_x > 0)
                        self.cursor_x -= self.glyph_width;
                break;

        case '\t':
                self.cursor_x += self.glyph_width * 4;
                break;
        
        default:
                self.draw_char_at(ch, self.cursor_x, self.cursor_y);
                self.cursor_x += self.glyph_width;

                if (self.cursor_x + self.glyph_width > self.width) {
                        self.cursor_x = 0;
                        self.cursor_y += self.glyph_height;
                }

                break;
        }

        if (self.cursor_y + self.glyph_height > self.height) {
                // TODO
        }
}

bool Console::is_active(this const Console &self)
{
        return self.active;
}

void Console::draw_char_at(this Console &self, char ch, int x, int y)
{
        u32 idx = static_cast<u8>(ch);
        u32 bytes_per_row = (self.glyph_width + 7) / 8;
        const u8 *glyph = self.font_data.get_data() + self.glyph_offset + idx * self.glyph_size;

        for (u32 _y = 0; _y < self.glyph_height; _y++) {
                for (u32 _x = 0; _x < self.glyph_width; _x++) {
                        u8 byte = glyph[_y * bytes_per_row + _x / 8];

                        if (byte & (0x80 >> (_x % 8)))
                                drivers::framebuffer::draw_pixel(x + _x, y + _y, 0xffffff);
                }
        }
}
// -------------------------------------------------------------------------

// Functions
// -------------------------------------------------------------------------
void set_console(Console console)
{
        current_console = console;
}

Console *get_console()
{
        return &current_console;
}
// -------------------------------------------------------------------------

} /* kernel::drivers::console */