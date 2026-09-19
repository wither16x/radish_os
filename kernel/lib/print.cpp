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
                                        usize color_idx = charToInt<usize>(color_ch);
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

        void printString(const String<> &s)
        {
                for (auto &ch : s)
                        putchar(ch);
        }
} // namespace Kiwi::Lib