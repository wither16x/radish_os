#pragma once

#include <lib/typing.hpp>

namespace Kiwi::Cpu
{
        /// Wrapper around the `HLT` instruction.
        inline void hlt()
        {
                __asm__ volatile ("hlt");
        }

        /// Wrapper around the `OUTB` instruction.
        inline void outb(Lib::u16 port, Lib::u8 byte)
        {
                __asm__ volatile ("outb %b0, %w1" :: "a"(byte), "Nd"(port) : "memory");
        }

        /// Wrapper around the `INB` instruction.
        inline Lib::u8 inb(Lib::u16 port)
        {
                Lib::u8 byte;
                __asm__ volatile ("inb %w1, %b0" : "=a"(byte) : "Nd"(port) : "memory");
                return byte;
        }

        /// Wrapper around the `INVLPG` instruction.
        inline void invlpg(Lib::uptr page)
        {
                __asm__ volatile ("invlpg (%0)" :: "r"(page) : "memory");
        }

        /// Wrapper around the `CLI` instruction.
        inline void cli()
        {
                __asm__ volatile ("cli");
        }

        /// Wrapper around the `STI` instruction.
        inline void sti()
        {
                __asm__ volatile ("sti");
        }
} // namespace Kiwi::Cpu