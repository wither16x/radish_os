#include <drivers/console.hpp>
#include <drivers/framebuffer.hpp>
#include <lib/filesystem.hpp>
#include <lib/logging.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>

using kernel::lib::String;
using kernel::lib::read_file, kernel::lib::get_file_size;
using kernel::lib::u8, kernel::lib::u32, kernel::lib::usize;

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
void Console::init_font(const String &font)
{
        logger.set_context("console");

        this->cursor_x = 0;
        this->cursor_y = 0;

        usize filesz = get_file_size(font);
        this->font_data.resize(filesz);
        read_file(font, reinterpret_cast<char *>(this->font_data.get_data()), filesz);

        PSF2Header *hdr = reinterpret_cast<PSF2Header *>(this->font_data.get_data());
        logger.debug("magic: 0x%x", hdr->magic);
        logger.debug("version: %u", hdr->version);
        logger.debug("header size: %u", hdr->hdrsize);
        logger.debug("flags: %u", hdr->flags);
        logger.debug("glyphs: %u", hdr->length);
        logger.debug("glyph size: %u", hdr->glyph_size);
        logger.debug("width: %u", hdr->width);
        logger.debug("height: %u", hdr->height);

        if (hdr->magic != PSF2Magic) {
                logger.err("invalid PSF2 magic dword: 0x%x", hdr->magic);
                logger.set_context("kernel");
                return;
        }

        this->glyph_offset = hdr->hdrsize;
        this->glyph_size = hdr->glyph_size;
        this->glyph_width = hdr->width;
        this->glyph_height = hdr->height;

        logger.set_context("kernel");
}

void Console::draw_char(char ch, int x, int y)
{
        u32 idx = static_cast<u8>(ch);
        u32 bytes_per_row = (this->glyph_width + 7) / 8;
        const u8 *glyph = this->font_data.get_data() + this->glyph_offset + idx * this->glyph_size;

        for (u32 _y = 0; _y < this->glyph_height; _y++) {
                for (u32 _x = 0; _x < this->glyph_width; _x++) {
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