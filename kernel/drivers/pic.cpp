#include <drivers/pic.hpp>
#include <cpu/io.hpp>
#include <lib/typing.hpp>

using kernel::lib::u8, kernel::lib::u16;

namespace kernel::drivers::pic {

namespace {

constexpr u16 PIC_MASTER        = 0x20;
constexpr u16 PIC_SLAVE         = 0xa0;

constexpr u16 MASTER_OFFSET     = 0x20;
constexpr u16 SLAVE_OFFSET      = 0x28;

/// Enumeration of the PIC ports.
enum Port : u16 {
        MASTER_COMMAND           = PIC_MASTER,
        MASTER_DATA              = PIC_MASTER + 1,

        SLAVE_COMMAND            = PIC_SLAVE,
        SLAVE_DATA               = PIC_SLAVE + 1
};

/// Enumeration of the PIC commands.
enum Command : u8 {
        // Note: ICW stands for Initialization Command Word
        ICW1_ICW4                = 0x01,
        ICW1_SINGLE              = 0x02,
        ICW1_INIT                = 0x10,
        ICW3_MASTER              = 0x04,
        ICW3_SLAVE               = 0x02,
        ICW4_8086                = 0x01,
        EOI                      = 0x20,  // end of interrupt,
        MASK_ALL                 = 0xff,
        UNMASK_ALL               = 0x00
};

} /* anonymous namespace */

// --------------------------------------------------
void remap()
{
        // start initialization
        cpu::output_byte_port(Port::MASTER_COMMAND, Command::ICW1_INIT | Command::ICW1_ICW4);
        cpu::output_byte_port(Port::SLAVE_COMMAND, Command::ICW1_INIT | Command::ICW1_ICW4);

        cpu::output_byte_port(Port::MASTER_DATA, MASTER_OFFSET);
        cpu::output_byte_port(Port::SLAVE_DATA, SLAVE_OFFSET);

        // tell the master that it has a slave
        cpu::output_byte_port(Port::MASTER_DATA, Command::ICW3_MASTER);
        cpu::output_byte_port(Port::SLAVE_DATA, Command::ICW3_SLAVE);

        // enable 8086 mode
        cpu::output_byte_port(Port::MASTER_DATA, Command::ICW4_8086);
        cpu::output_byte_port(Port::SLAVE_DATA, Command::ICW4_8086);
}
// --------------------------------------------------

// --------------------------------------------------
void send_eoi(u8 irq)
{
        if (irq >= 8)
                cpu::output_byte_port(Port::SLAVE_COMMAND, Command::EOI);
        else
                cpu::output_byte_port(Port::MASTER_COMMAND, Command::EOI);
}
// --------------------------------------------------

// --------------------------------------------------
void irq_mask(lib::u8 irq)
{
        Port port;
        Command cmd;

        if (irq < 8) {
                port = Port::MASTER_DATA;
        } else {
                port = Port::SLAVE_DATA;
                irq -= 8;
        }

        cmd = static_cast<Command>(
                cpu::input_byte_port(port) | (1 << irq)
        );
        cpu::output_byte_port(port, cmd);
}
// --------------------------------------------------

// --------------------------------------------------
void irq_unmask(lib::u8 irq)
{
        Port port;
        Command cmd;

        if (irq < 8) {
                port = Port::MASTER_DATA;
        } else {
                port = Port::SLAVE_DATA;
                irq -= 8;
        }

        cmd = static_cast<Command>(
                cpu::input_byte_port(port) & ~(1 << irq)
        );
        cpu::output_byte_port(port, cmd);
}
// --------------------------------------------------

// --------------------------------------------------
void irq_mask_all()
{
        cpu::output_byte_port(Port::MASTER_DATA, Command::MASK_ALL);
        cpu::output_byte_port(Port::SLAVE_DATA, Command::MASK_ALL);
}
// --------------------------------------------------

// --------------------------------------------------
void irq_unmask_all()
{
        cpu::output_byte_port(Port::MASTER_DATA, Command::UNMASK_ALL);
        cpu::output_byte_port(Port::SLAVE_DATA, Command::UNMASK_ALL);
}
// --------------------------------------------------

} /* namespace kernel::drivers::pic */