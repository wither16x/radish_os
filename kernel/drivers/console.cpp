#include <drivers/console.hpp>
#include <drivers/framebuffer.hpp>
#include <fs/devfs.hpp>
#include <lib/filesystem.hpp>
#include <lib/logging.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>

using kernel::lib::String;
using kernel::lib::read, kernel::lib::getfilesz;
using kernel::lib::u8, kernel::lib::u32, kernel::lib::u64, kernel::lib::usize;
using kernel::lib::Vector;

using kernel::lib::log::logger;

namespace kernel::drivers::console {

namespace {

constexpr u32 PSF2_MAGIC = 0x864ab572;

/// Representation of a PSF2 header.
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

enum Char : char {
        CH_CR           = '\r',
        CH_NL           = '\n',
        CH_TAB          = '\t',
        CH_BS           = '\b',
        CH_DEL          = 0x7f
};

Console current_console;

} /* anonymous namespace */

// --------------------------------------------------
Console::Console(u64 width, u64 height)
{
        this->active    = false;
        this->width     = width;
        this->height    = height;
        this->cursor_x  = 0;
        this->cursor_y  = 0;

        fs::devfs::register_device(fs::devfs::DeviceType::Console, "D:/console");
}
// --------------------------------------------------

// --------------------------------------------------
Console::~Console()
{
        this->active = false;
}
// --------------------------------------------------

/// Here we assume that the provided font is using the PSF2
/// format and does not have an Unicode table.
// --------------------------------------------------
void Console::init_font(this Console &self, const String &font)
{
        logger.set_context("console");

        usize filesz = 0;
        getfilesz(font, &filesz);
        self.font_data.resize(filesz);

        read(font, reinterpret_cast<char *>(self.font_data.get_data()), filesz);

        PSF2Header *hdr = reinterpret_cast<PSF2Header *>(self.font_data.get_data());

        if (hdr->magic != PSF2_MAGIC) {
                logger.err("invalid PSF2 magic dword: 0x%x", hdr->magic);
                logger.set_context("kernel");
                return;
        }

        self.glyph_offset       = hdr->hdrsize;
        self.glyph_size         = hdr->glyph_size;
        self.glyph_width        = hdr->width;
        self.glyph_height       = hdr->height;
        // the console is active only if a valid font has been loaded
        self.active             = true;

        logger.set_context("kernel");
}
// --------------------------------------------------

// --------------------------------------------------
void Console::draw_char(this Console &self, char ch, u32 color)
{
        switch (ch) {
        case Char::CH_CR:
                self.cursor_x = 0;
                break;

        case Char::CH_NL:
                self.cursor_y += self.glyph_height;
                self.cursor_x = 0;
                break;

        case Char::CH_BS:
                if (self.cursor_x > 0)
                        self.cursor_x -= self.glyph_width;
                break;

        case Char::CH_TAB:
                self.cursor_x += self.glyph_width * 4;
                break;

        case Char::CH_DEL:
                if (self.cursor_x > 0) {
                        self.cursor_x -= self.glyph_width;
                        framebuffer::draw_rectangle(self.cursor_x, self.cursor_y, self.glyph_width, self.glyph_height, 0);
                }
                break;
        
        default:
                self.draw_char_at(ch, self.cursor_x, self.cursor_y, color);
                self.cursor_x += self.glyph_width;

                if (self.cursor_x + self.glyph_width > self.width) {
                        self.cursor_x = 0;
                        self.cursor_y += self.glyph_height;
                }

                break;
        }

        if (self.cursor_y + self.glyph_height > self.height)
                self.scroll();
}
// --------------------------------------------------

// --------------------------------------------------
bool Console::is_active(this const Console &self)
{
        return self.active;
}
// --------------------------------------------------

// --------------------------------------------------
const Vector<u8> &Console::get_font_data(this const Console &self)
{
        return self.font_data;
}
// --------------------------------------------------

// --------------------------------------------------
void Console::draw_char_at(this Console &self, char ch, int px, int py, u32 color)
{
        u32 idx = static_cast<u8>(ch);
        u32 bytes_per_row = (self.glyph_width + 7) / 8;
        const u8 *glyph = self.font_data.get_data() + self.glyph_offset + idx * self.glyph_size;

        for (u32 y = 0; y < self.glyph_height; y++) {
                for (u32 x = 0; x < self.glyph_width; x++) {
                        u8 byte = glyph[y * bytes_per_row + x / 8];

                        if (byte & (0x80 >> (x % 8)))
                                drivers::framebuffer::draw_pixel(px + x, py + y, color);
                }
        }
}
// --------------------------------------------------

// --------------------------------------------------
void Console::scroll(this Console &self)
{
        framebuffer::scroll(self.glyph_height);
        self.cursor_y -= self.glyph_height;
}
// --------------------------------------------------

// --------------------------------------------------
void set_console(const Console &console)
{
        current_console = console;
}
// --------------------------------------------------

// --------------------------------------------------
Console &get_console()
{
        return current_console;
}
// --------------------------------------------------

} /* kernel::drivers::console */