#include <cpu/io.hpp>
#include <cpu/assembly.hpp>
#include <drivers/serial.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Drivers::Serial
{
        namespace
        {
                /// Wait while a port is busy.
                void waitPortBusy(Lib::u16 port)
                {
                        while ((Cpu::inputBytePort(port + 5) & 0x20) == 0);
                }
        } // anonymous namespace

        bool initPort(Lib::u16 port)
        {
                Cpu::outputBytePort(port + 1, 0);
                Cpu::outputBytePort(port + 3, 0x80);
                Cpu::outputBytePort(port, 0x03);
                Cpu::outputBytePort(port + 1, 0x00);
                Cpu::outputBytePort(port + 3, 0x03);
                Cpu::outputBytePort(port + 2, 0xc7);
                Cpu::outputBytePort(port + 4, 0x0b);
                Cpu::outputBytePort(port + 4, 0x1e);

                Cpu::outputBytePort(port, 0xae);
                if (Cpu::inputBytePort(port) != 0xae)
                        return false;

                Cpu::outputBytePort(port + 4, 0xf);
                return true;
        }

        void sendByte(Lib::u16 port, Lib::u8 byte)
        {
                waitPortBusy(port);
                Cpu::outputBytePort(port, byte);
        }
} // namespace Kiwi::Drivers::Serial