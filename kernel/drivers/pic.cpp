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

enum Port : u16 {
        MasterCommand           = PIC_MASTER,
        MasterData              = PIC_MASTER + 1,

        SlaveCommand            = PIC_SLAVE,
        SlaveData               = PIC_SLAVE + 1
};

enum Command : u8 {
        // Note: ICW stands for Initialization Command Word
        ICW1ICW4                = 0x01,
        ICW1Single              = 0x02,
        ICW1Init                = 0x10,
        ICW3Master              = 0x04,
        ICW3Slave               = 0x02,
        ICW48086                = 0x01,
        EOI                     = 0x20,  // end of interrupt,
        MaskAll                 = 0xff,
        UnmaskAll               = 0x00
};

} /* anonymous namespace */

void remap()
{
        // start initialization
        cpu::output_byte_port(Port::MasterCommand, Command::ICW1Init | Command::ICW1ICW4);
        cpu::output_byte_port(Port::SlaveCommand, Command::ICW1Init | Command::ICW1ICW4);

        cpu::output_byte_port(Port::MasterData, MASTER_OFFSET);
        cpu::output_byte_port(Port::SlaveData, SLAVE_OFFSET);

        // tell the master that it has a slave
        cpu::output_byte_port(Port::MasterData, Command::ICW3Master);
        cpu::output_byte_port(Port::SlaveData, Command::ICW3Slave);

        // enable 8086 mode
        cpu::output_byte_port(Port::MasterData, Command::ICW48086);
        cpu::output_byte_port(Port::SlaveData, Command::ICW48086);
}

void send_eoi(u8 irq)
{
        if (irq >= 8)
                cpu::output_byte_port(Port::SlaveCommand, Command::EOI);
        else
                cpu::output_byte_port(Port::MasterCommand, Command::EOI);
}

void irq_mask(lib::u8 irq)
{
        Port port;
        Command cmd;

        if (irq < 8) {
                port = Port::MasterData;
        } else {
                port = Port::SlaveData;
                irq -= 8;
        }

        cmd = static_cast<Command>(
                cpu::input_byte_port(port) | (1 << irq)
        );
        cpu::output_byte_port(port, cmd);
}

void irq_unmask(lib::u8 irq)
{
        Port port;
        Command cmd;

        if (irq < 8) {
                port = Port::MasterData;
        } else {
                port = Port::SlaveData;
                irq -= 8;
        }

        cmd = static_cast<Command>(
                cpu::input_byte_port(port) & ~(1 << irq)
        );
        cpu::output_byte_port(port, cmd);
}

void irq_mask_all()
{
        cpu::output_byte_port(Port::MasterData, Command::MaskAll);
        cpu::output_byte_port(Port::SlaveData, Command::MaskAll);
}

void irq_unmask_all()
{
        cpu::output_byte_port(Port::MasterData, Command::UnmaskAll);
        cpu::output_byte_port(Port::SlaveData, Command::UnmaskAll);
}

} /* namespace kernel::drivers::pic */