#include <cpu/assembly.hpp>
#include <drivers/serial.hpp>
#include <lib/typing.hpp>

using kernel::lib::u8, kernel::lib::u16;
using kernel::lib::Status;

namespace kernel::drivers::serial {

namespace {

void outb_to_port(Port port, u8 byte, u16 offset = 0)
{
        cpu::outb(static_cast<u16>(port) + offset, byte);
}

u8 inb_to_port(Port port, u16 offset = 0)
{
        return cpu::inb(static_cast<u16>(port) + offset);
}

void wait_port_busy(Port port)
{
        while ((inb_to_port(port, 5) & 0x20) == 0);
}

}

Status init_port(Port port)
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
                return Status::Err;

        outb_to_port(port, 0xf, 4);
        return Status::Ok;
}

void send_byte(Port port, u8 byte)
{
        wait_port_busy(port);
        outb_to_port(port, byte);
}

} /* namespace kernel::drivers::serial */