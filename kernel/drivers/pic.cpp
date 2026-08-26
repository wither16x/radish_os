#include <drivers/pic.hpp>
#include <cpu/io.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Drivers::Pic
{
        namespace
        {
                constexpr Lib::u16 PIC_MASTER        = 0x20;
                constexpr Lib::u16 PIC_SLAVE         = 0xa0;

                constexpr Lib::u16 MASTER_OFFSET     = 0x20;
                constexpr Lib::u16 SLAVE_OFFSET      = 0x28;

                /// Enumeration of the PIC ports.
                enum Port : Lib::u16
                {
                        MASTER_COMMAND           = PIC_MASTER,
                        MASTER_DATA              = PIC_MASTER + 1,

                        SLAVE_COMMAND            = PIC_SLAVE,
                        SLAVE_DATA               = PIC_SLAVE + 1
                };

                /// Enumeration of the PIC commands.
                enum Command : Lib::u8
                {
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
        } // anonymous namespace

        void remap()
        {
                // start initialization
                Cpu::ioWait();
                Cpu::outputBytePort(Port::MASTER_COMMAND, Command::ICW1_INIT | Command::ICW1_ICW4);
                Cpu::ioWait();
                Cpu::outputBytePort(Port::SLAVE_COMMAND, Command::ICW1_INIT | Command::ICW1_ICW4);

                Cpu::ioWait();
                Cpu::outputBytePort(Port::MASTER_DATA, MASTER_OFFSET);
                Cpu::ioWait();
                Cpu::outputBytePort(Port::SLAVE_DATA, SLAVE_OFFSET);

                // tell the master that it has a slave
                Cpu::ioWait();
                Cpu::outputBytePort(Port::MASTER_DATA, Command::ICW3_MASTER);
                Cpu::ioWait();
                Cpu::outputBytePort(Port::SLAVE_DATA, Command::ICW3_SLAVE);

                // enable 8086 mode
                Cpu::ioWait();
                Cpu::outputBytePort(Port::MASTER_DATA, Command::ICW4_8086);
                Cpu::ioWait();
                Cpu::outputBytePort(Port::SLAVE_DATA, Command::ICW4_8086);
        }

        void sendEoi(Lib::u8 irq)
        {
                if (irq >= 8)
                        Cpu::outputBytePort(Port::SLAVE_COMMAND, Command::EOI);
                else
                        Cpu::outputBytePort(Port::MASTER_COMMAND, Command::EOI);
        }

        void irqMask(Lib::u8 irq)
        {
                Port port;
                Lib::u8 cmd;

                if (irq < 8) {
                        port = Port::MASTER_DATA;
                } else {
                        port = Port::SLAVE_DATA;
                        irq -= 8;
                }

                cmd = Cpu::inputBytePort(port) | (1 << irq);
                Cpu::outputBytePort(port, cmd);
        }

        void irqUnmask(Lib::u8 irq)
        {
                Port port;
                Lib::u8 cmd;

                if (irq < 8) {
                        port = Port::MASTER_DATA;
                } else {
                        port = Port::SLAVE_DATA;
                        irq -= 8;
                }

                cmd = Cpu::inputBytePort(port) & ~(1 << irq);
                Cpu::outputBytePort(port, cmd);
        }

        void irqMaskAll()
        {
                Cpu::outputBytePort(Port::MASTER_DATA, Command::MASK_ALL);
                Cpu::outputBytePort(Port::SLAVE_DATA, Command::MASK_ALL);
        }

        void irqUnmaskAll()
        {
                Cpu::outputBytePort(Port::MASTER_DATA, Command::UNMASK_ALL);
                Cpu::outputBytePort(Port::SLAVE_DATA, Command::UNMASK_ALL);
        }
} // namespace Kiwi::Drivers::Pic