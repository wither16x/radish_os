#include <cpu/io.hpp>
#include <drivers/keyboard.hpp>
#include <drivers/ps2kbd.hpp>
#include <lib/typing.hpp>

using kernel::lib::u8, kernel::lib::usize;;

namespace kernel::drivers::keyboard {

namespace {

constexpr usize MAX_KEYS = 128;

struct Key {
        u8 scancode;
        char unshift;
        char shift;
};

char unshift_kbd_layout[MAX_KEYS] = {};
char shift_kbd_layout[MAX_KEYS] = {};

constexpr Key keys[] = {
        { 0x0e, '`',  '~'},
        { 0x16, '1',  '!'},
        { 0x1e, '2',  '@'},
        { 0x26, '3',  '#'},
        { 0x25, '4',  '$'},
        { 0x2e, '5',  '%'},
        { 0x36, '6',  '^'},
        { 0x3d, '7',  '&'},
        { 0x3e, '8',  '*'},
        { 0x46, '9',  '('},
        { 0x45, '0',  ')'},
        { 0x4e, '-',  '_'},
        { 0x55, '=',  '+'},
        { 0x66, '\b', '\b'},
        { 0x0d, '\t', '\t'},
        { 0x15, 'q',  'Q'},
        { 0x1d, 'w',  'W'},
        { 0x24, 'e',  'E'},
        { 0x2d, 'r',  'R'},
        { 0x2c, 't',  'T'},
        { 0x35, 'y',  'Y'},
        { 0x3c, 'u',  'U'},
        { 0x43, 'i',  'I'},
        { 0x44, 'o',  'O'},
        { 0x4d, 'p',  'P'},
        { 0x54, '[',  '{'},
        { 0x5b, ']',  '}'},
        { 0x5d, '\\', '|'},
        { 0x1c, 'a',  'A'},
        { 0x1b, 's',  'S'},
        { 0x23, 'd',  'D'},
        { 0x2b, 'f',  'F'},
        { 0x34, 'g',  'G'},
        { 0x33, 'h',  'H'},
        { 0x3b, 'j',  'J'},
        { 0x42, 'k',  'K'},
        { 0x4b, 'l',  'L'},
        { 0x4c, ';',  ':'},
        { 0x52, '\'', '"'},
        { 0x5a, '\n', '\n'},
        { 0x1a, 'z',  'Z'},
        { 0x22, 'x',  'X'},
        { 0x21, 'c',  'C'},
        { 0x2a, 'v',  'V'},
        { 0x32, 'b',  'B'},
        { 0x31, 'n',  'N'},
        { 0x3a, 'm',  'M'},
        { 0x41, ',',  '<'},
        { 0x49, '.',  '>'},
        { 0x4a, '/',  '?'},
        { 0x29, ' ',  ' '}
};

bool shift = false;
bool expecting_break = false;

} /* anonymous namespace */

void init()
{
        for (const Key &k : keys) {
                unshift_kbd_layout[k.scancode] = k.unshift;
                shift_kbd_layout[k.scancode] = k.shift;
        }
}

u8 handle_key_press()
{
        u8 status = cpu::input_byte_port(ps2kbd::ControllerPort::PORT_STATUS);

        u8 k = 0;
        if (status & ps2kbd::ControllerStatus::ST_OUTPUT_BUFFER_STATUS)
                k = cpu::input_byte_port(ps2kbd::ControllerPort::PORT_DATA);

        return k;
}

char scancode_to_key(lib::u8 scancode)
{
        if (scancode == 0xf0) {
                expecting_break = true;
                return '\0';
        }

        if (expecting_break) {
                expecting_break = false;
                if (scancode == 0x12 || scancode == 0x59)
                        shift = false;
                return '\0';
        }

        if (scancode == 0x12 || scancode == 0x59) {
                shift = true;
                return '\0';
        }

        if (scancode >= MAX_KEYS)
                return '\0';

        if (shift)
                return shift_kbd_layout[scancode];
        else
                return unshift_kbd_layout[scancode];
}

} /* namespace kernel::drivers::keyboard */