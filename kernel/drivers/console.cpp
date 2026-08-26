#include <drivers/console.hpp>
#include <drivers/framebuffer.hpp>
#include <fs/devfs.hpp>
#include <lib/filesystem.hpp>
#include <lib/logging.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>

namespace Kiwi::Drivers::Console
{
        namespace
        {
                constexpr Lib::u32 PSF2_MAGIC = 0x864ab572;

                /// Representation of a PSF2 header.
                struct Psf2Header
                {
                        Lib::u32 magic;
                        Lib::u32 version;            // currently always 0 (source: wikipedia)
                        Lib::u32 hdrsize;
                        Lib::u32 flags;
                        Lib::u32 length;             // number of glyphs
                        Lib::u32 glyph_size;         // bytes per glyph
                        Lib::u32 height;             // glyph height
                        Lib::u32 width;              // glyph width
                };

                enum Char : char
                {
                        CH_CR           = '\r',
                        CH_NL           = '\n',
                        CH_TAB          = '\t',
                        CH_BS           = '\b',
                        CH_DEL          = 0x7f
                };

                Console current_console;
        } // anonymous namespace

        Console::Console(Lib::u64 width, Lib::u64 height)
        {
                this->active    = false;
                this->width     = width;
                this->height    = height;
                this->cursor_x  = 0;
                this->cursor_y  = 0;

                Fs::Devfs::registerDevice(Fs::Devfs::DeviceType::Console, "D:/console");
        }

        Console::~Console()
        {
                this->active = false;
        }

        /// Here we assume that the provided font is using the PSF2
        /// format and does not have an Unicode table.
        void Console::initFont(this Console &self, const Lib::String &font)
        {
                Lib::Log::logger.set_context("console");

                Lib::usize filesz = 0;
                getfilesz(font, &filesz);
                self.font_data.resize(filesz);

                Lib::File *font_file = open(font);
                Lib::read(font_file, reinterpret_cast<char *>(self.font_data.getData()), filesz);
                Lib::close(font_file);

                Psf2Header *hdr = reinterpret_cast<Psf2Header *>(self.font_data.getData());

                if (hdr->magic != PSF2_MAGIC) {
                        Lib::Log::logger.err("invalid PSF2 magic dword: 0x%x", hdr->magic);
                        Lib::Log::logger.set_context("kernel");
                        return;
                }

                self.glyph_offset       = hdr->hdrsize;
                self.glyph_size         = hdr->glyph_size;
                self.glyph_width        = hdr->width;
                self.glyph_height       = hdr->height;
                // the console is active only if a valid font has been loaded
                self.active             = true;

                Lib::Log::logger.set_context("kernel");
        }

        void Console::drawChar(this Console &self, char ch, Lib::u32 color)
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
                                Framebuffer::drawRectangle(self.cursor_x, self.cursor_y, self.glyph_width, self.glyph_height, 0);
                        }
                        break;
                
                default:
                        self.drawCharAt(ch, self.cursor_x, self.cursor_y, color);
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

        bool Console::isActive(this const Console &self)
        {
                return self.active;
        }

        const Lib::Vector<Lib::u8> &Console::getFontData(this const Console &self)
        {
                return self.font_data;
        }

        void Console::drawCharAt(this Console &self, char ch, int px, int py, Lib::u32 color)
        {
                Lib::u32 idx = static_cast<Lib::u8>(ch);
                Lib::u32 bytes_per_row = (self.glyph_width + 7) / 8;
                const Lib::u8 *glyph = self.font_data.getData() + self.glyph_offset + idx * self.glyph_size;

                for (Lib::u32 x = 0; x < self.glyph_width; x++) {
                        for (Lib::u32 y = 0; y < self.glyph_height; y++) {
                                Lib::u8 byte = glyph[y * bytes_per_row + x / 8];

                                if (byte & (0x80 >> (x % 8)))
                                        Framebuffer::drawPixel(px + x, py + y, color);
                        }
                }
        }

        void Console::scroll(this Console &self)
        {
                Framebuffer::scroll(self.glyph_height);
                self.cursor_y -= self.glyph_height;
        }

        void setConsole(const Console &console)
        {
                current_console = console;
        }

        Console &getConsole()
        {
                return current_console;
        }
} // Kiwi::Drivers::Console