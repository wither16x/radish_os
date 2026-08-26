#pragma once

#include <cpu/assembly.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Cpu
{
        // NOTE: it is recommended to use the functions implemented
        // below instead of using the assembly wrappers directly as
        // they can wrap several assembly wrappers if needed.

        /// Output a byte to a port.
        inline void outputBytePort(Lib::u16 port, Lib::u8 byte)
        {
                outb(port, byte);
        }

        /// Get a byte from a port.
        inline Lib::u8 inputBytePort(Lib::u16 port)
        {
                return inb(port);
        }

        /// Small delay.
        inline void ioWait()
        {
                outb(0x80, 0);
        }
} // namespace Kiwi::Cpu