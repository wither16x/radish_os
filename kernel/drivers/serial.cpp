#include <cpu/assembly.hpp>
#include <drivers/serial.hpp>
#include <lib/typing.hpp>

namespace kernel {

namespace drivers {

namespace serial {

namespace {

void outb_to_port(Port port, lib::u8 byte, lib::u16 offset = 0)
{
        cpu::outb(static_cast<lib::u16>(port) + offset, byte);
}

lib::u8 inb_to_port(Port port, lib::u16 offset = 0)
{
        return cpu::inb(static_cast<lib::u16>(port) + offset);
}

void wait_port_busy(Port port)
{
        while ((inb_to_port(port, 5) & 0x20) == 0);
}

}

lib::Status init_port(Port port)
{
        outb_to_port(port, 0, 1);
	outb_to_port(port, 0x80, 3);
	outb_to_port(port, 0x03);
	outb_to_port(port, 0x00, 1);
	outb_to_port(port, 0x03, 3);
	outb_to_port(port, 0xc7, 2);
	outb_to_port(port, 0x0b, 4);
	outb_to_port(port, 0x1e, 4);

	outb_to_port(port, 0xae);
        if (inb_to_port(port) != 0xae)
                return lib::Status::Err;

        outb_to_port(port, 0xf, 4);
        return lib::Status::Ok;
}

void send_byte(Port port, lib::u8 byte)
{
        wait_port_busy(port);
        outb_to_port(port, byte);
}

} /* namespace serial */

} /* namespace drivers */

} /* namespace kernel */