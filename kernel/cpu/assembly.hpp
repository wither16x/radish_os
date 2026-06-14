#pragma once

#include <lib/typing.hpp>

namespace kernel::cpu {

inline void hlt()
{
        __asm__ volatile ("hlt");
}

inline void outb(lib::u16 port, lib::u8 byte)
{
        __asm__ volatile ("outb %b0, %w1" :: "a"(byte), "Nd"(port) : "memory");
}

inline lib::u8 inb(lib::u16 port)
{
        lib::u8 byte;
        __asm__ volatile ("inb %w1, %b0" : "=a"(byte) : "Nd"(port) : "memory");
        return byte;
}

inline void invlpg(lib::uptr page)
{
        __asm__ volatile ("invlpg (%0)" :: "r"(page) : "memory");
}

inline void cli()
{
        __asm__ volatile ("cli");
}

inline void sti()
{
        __asm__ volatile ("sti");
}

} /* namespace kernel::cpu */