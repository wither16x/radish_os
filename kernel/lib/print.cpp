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

} /* namespace lib */

} /* namespace kernel */