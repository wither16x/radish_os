#pragma once

#include <lib/typing.hpp>

namespace kernel::cpu {

/// Wrapper around the `HLT` instruction.
inline void hlt()
{
        __asm__ volatile ("hlt");
}

/// Wrapper around the `OUTB` instruction.
inline void outb(lib::u16 port, lib::u8 byte)
{
        __asm__ volatile ("outb %b0, %w1" :: "a"(byte), "Nd"(port) : "memory");
}

/// Wrapper around the `INB` instruction.
inline lib::u8 inb(lib::u16 port)
{
        lib::u8 byte;
        __asm__ volatile ("inb %w1, %b0" : "=a"(byte) : "Nd"(port) : "memory");
        return byte;
}

/// Wrapper around the `INVLPG` instruction.
inline void invlpg(lib::uptr page)
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

} /* namespace kernel::cpu */