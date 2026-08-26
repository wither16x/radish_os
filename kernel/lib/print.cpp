#include <drivers/console.hpp>
#include <drivers/serial.hpp>
#include <lib/conversion.hpp>
#include <lib/print.hpp>
#include <lib/string.hpp>

namespace Kiwi::Lib
{
        namespace
        {
                constexpr int ANSI_COLOR_COUNT = 8;

                bool ansi_seq_start = false;
                // "sqb" stands for "square bracket"
                bool found_left_sqb = false;
                bool color_start = false;
                bool end_of_ansi_sequence = false;
                u32 color = 0xffffff;
                int color_ch;

                u32 ansi_colors[ANSI_COLOR_COUNT] = {
                        0x000000, // black
                        0xfc0303, // red
                        0x39fc03, // green
                        0xfcf403, // yellow
                        0x0b03fc, // blue
                        0xfc03e8, // cyan
                        0x00b7ff, // cyan
                        0xffffff  // white
                };

                /// Only supports regular colors.
                u32 processAnsiSequence(int ch)
                {
                        if (ch == '\033') {
                                ansi_seq_start = true;
                                return color;
                        }

                        if (ch == '[' && ansi_seq_start) {
                                found_left_sqb = true;
                                return color;
                        }

                        if (found_left_sqb) {
                                if (end_of_ansi_sequence) {
                                        usize color_idx = ctoi(color_ch);
                                        found_left_sqb = false;
                                        ansi_seq_start = false;
                                        end_of_ansi_sequence = false;
                                        if (color_idx < ANSI_COLOR_COUNT)
                                                return ansi_colors[color_idx];
                                        return color;
                                }

                                if (ch == 'm' && color_start) {
                                        end_of_ansi_sequence = true;
                                        color_start = false;
                                        return color;
                                }

                                if (ch == '3') {
                                        color_start = true;
                                        return color;
                                }

                                color_ch = ch;
                        }

                        return color;
                }
        } // anonymous namespace

        void putchar(int ch)
        {
                color = processAnsiSequence(ch);
                if (ch == '\033')
                        return;
                if (ansi_seq_start and '0' <= ch and ch <= '7')
                        return;
                if (ansi_seq_start and (ch == 'm' or ch == '['))
                        return;

                Drivers::Console::Console &console = Drivers::Console::getConsole();
                if (console.isActive())
                        console.drawChar(ch, color);
                else
                        Drivers::Serial::sendByte(Drivers::Serial::Port::SERIAL_COM1, ch);
        }

        void printString(const char *s)
        {
                while (*s)
                        putchar(*s++);
        }

        void vprintf(const char *fmt, va_list args)
        {
                char buf[65];

                while (*fmt) {
                        if (*fmt != '%') {
                                putchar(*fmt);
                                fmt++;
                                continue;
                        }

                        fmt++;          // eat the '%'
                        switch(*fmt) {
                        case 'c': {
                                int ch = va_arg(args, int);
                                putchar(ch);
                                break;
                        }

                        case 'b': {
                                usize n = va_arg(args, usize);
                                char *s = utoa(n, buf, 2);
                                printString(s);
                                break;
                        }

                        case 'o': {
                                usize n = va_arg(args, usize);
                                char *s = utoa(n, buf, 8);
                                printString(s);
                                break;
                        }

                        case 'd': {
                                isize n = va_arg(args, isize);
                                char *s = itoa(n, buf, 10);
                                printString(s);
                                break;
                        }

                        case 'u': {
                                usize n = va_arg(args, usize);
                                char *s = utoa(n, buf, 10);
                                printString(s);
                                break;
                        }

                        case 'x': {
                                usize n = va_arg(args, usize);
                                char *s = utoa(n, buf, 16);
                                printString(s);
                                break;
                        }

                        case 'f': {
                                double n = va_arg(args, double);
                                char *s = ftoa(n, buf);
                                printString(s);
                                break;
                        }

                        case 's': {
                                const char *s = va_arg(args, const char *);
                                printString(s);
                                break;
                        }

                        default:
                                printString("<unsupported format>");
                                break;
                        }

                        fmt++;
                }
        }

        void printf(const char *fmt, ...)
        {
                va_list args;
                va_start(args, fmt);
                vprintf(fmt, args);
                va_end(args);
        }

        void println(const char *fmt, ...)
        {
                va_list args;
                va_start(args, fmt);
                vprintf(fmt, args);
                va_end(args);
                printString("\r\n");
        }
} // namespace Kiwi::Lib