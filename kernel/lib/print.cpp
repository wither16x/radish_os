#include <drivers/console.hpp>
#include <drivers/serial.hpp>
#include <lib/conversion.hpp>
#include <lib/print.hpp>
#include <lib/string.hpp>

namespace kernel::lib {

void putchar(int ch)
{
        drivers::console::Console *console = drivers::console::get_console();
        if (console->is_active())
                console->draw_char(ch);
        else
                drivers::serial::send_byte(drivers::serial::Port::COM1, ch);
}

void print_string(const char *s)
{
        while (*s)
                putchar(*s++);
}

void vprintf(const char *fmt, va_list args)
{
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
                        char *s = utoa(n, 2);
                        print_string(s);
                        break;
                }

                case 'o': {
                        usize n = va_arg(args, usize);
                        char *s = utoa(n, 8);
                        print_string(s);
                        break;
                }

                case 'd': {
                        isize n = va_arg(args, isize);
                        char *s = itoa(n, 10);
                        print_string(s);
                        break;
                }

                case 'u': {
                        usize n = va_arg(args, usize);
                        char *s = utoa(n, 10);
                        print_string(s);
                        break;
                }

                case 'x': {
                        usize n = va_arg(args, usize);
                        char *s = utoa(n, 16);
                        print_string(s);
                        break;
                }

                case 's': {
                        const char *s = va_arg(args, const char *);
                        print_string(s);
                        break;
                }

                default:
                        print_string("<unsupported format>");
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
        print_string("\r\n");
}

} /* namespace kernel::lib */