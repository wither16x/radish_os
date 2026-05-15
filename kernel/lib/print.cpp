#include <drivers/serial.hpp>
#include <lib/print.hpp>

namespace kernel {

namespace lib {

void putchar(int ch)
{
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
}

} /* namespace lib */

} /* namespace kernel */