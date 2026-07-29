#include <cpu/io.hpp>
#include <cpu/assembly.hpp>
#include <drivers/serial.hpp>
#include <lib/typing.hpp>

using kernel::lib::u8, kernel::lib::u16;

namespace kernel::drivers::serial {

namespace {

/// Wait while a port is busy.
void wait_port_busy(u16 port)
{
        while ((cpu::input_byte_port(port + 5) & 0x20) == 0);
}

} /* anonymous namespace */

// --------------------------------------------------
bool init_port(u16 port)
{
        cpu::output_byte_port(port + 1, 0);
	cpu::output_byte_port(port + 3, 0x80);
	cpu::output_byte_port(port, 0x03);
	cpu::output_byte_port(port + 1, 0x00);
	cpu::output_byte_port(port + 3, 0x03);
	cpu::output_byte_port(port + 2, 0xc7);
	cpu::output_byte_port(port + 4, 0x0b);
	cpu::output_byte_port(port + 4, 0x1e);

	cpu::output_byte_port(port, 0xae);
        if (cpu::input_byte_port(port) != 0xae)
                return false;

        cpu::output_byte_port(port + 4, 0xf);
        return true;
}
// --------------------------------------------------

// --------------------------------------------------
void send_byte(u16 port, u8 byte)
{
        wait_port_busy(port);
        cpu::output_byte_port(port, byte);
}
// --------------------------------------------------

} /* namespace kernel::drivers::serial */