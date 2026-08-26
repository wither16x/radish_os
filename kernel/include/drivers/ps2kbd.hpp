#pragma once

#include <cpu/io.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Drivers::Ps2Kbd
{
        enum ControllerPort : Lib::u16
        {
                PORT_DATA       = 0x60,         // RW
                PORT_STATUS     = 0x64,         // R
                PORT_CMD        = 0x64          // W
        };

        /// These flags are contained by the status register, represented here by
        /// `Port::PORT_STATUS`.
        enum ControllerStatus : Lib::u8
        {
                ST_OUTPUT_BUFFER_STATUS = 1 << 0,
                ST_INPUT_BUFFER_STATUS = 1 << 1,
                ST_SYSTEM_FLAG = 1 << 2,
                ST_CMD_DATA = 1 << 3,
                ST_UNKNOWN0 = 1 << 4,                    // chipset specific
                ST_UNKNOWN1 = 1 << 5,                    // chipset specific
                ST_TIMEOUT_ERROR = 1 << 6,
                ST_PARITY_ERROR = 1 << 7
        };

        void init();

        inline void waitOutputReady()
        {
                while (not (Cpu::inputBytePort(ControllerPort::PORT_STATUS) & ControllerStatus::ST_OUTPUT_BUFFER_STATUS));
        }

        inline void waitInputReady()
        {
                while (Cpu::inputBytePort(ControllerPort::PORT_STATUS) & ControllerStatus::ST_INPUT_BUFFER_STATUS);
        }

        /// Send a byte to the controller's command port.
        inline void sendCommand(Lib::u8 command)
        {
                waitInputReady();
                Cpu::outputBytePort(ControllerPort::PORT_CMD, command);
        }

        /// Read a byte from the controller's data port.
        Lib::u8 read();
        /// Write a byte to the controller's data port.
        void write(Lib::u8 byte);
        void flushOutputBuffer();
} // namespace Kiwi::Drivers::Ps2Kbd