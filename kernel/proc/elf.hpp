#pragma once

#include <lib/string.hpp>
#include <lib/typing.hpp>

namespace kernel::proc::elf {

enum MachineType : lib::u8 {
        Bit32   = 1,
        Bit64
};

enum EndianType : lib::u8 {
        LittleEndian = 1,
        BigEndian
};

enum ABIType : lib::u8 {
        SystemV
};

enum ELFType : lib::u8 {
        Relocatable = 1,
        Executable,
        Shared,
        Core
};

enum ELFVersion : lib::u32 {
        Version1 = 1
};

enum FlagType : lib::u32 {
        FExecutable = 1,
        FWritable,
        FReadable = 4
};

enum ISAType : lib::u16 {
        NoSpecific,
        Sparc           = 0x02,
        x86,
        MIPS            = 0x08,
        PARISC          = 0x0f,
        PowerPC32       = 0x14,
        PowerPC64       = 0x15,
        S390            = 0x16,
        ARM             = 0x28,
        Alpha           = 0x29,
        SuperH          = 0x2a,
        IA64            = 0x32,
        x86_64          = 0x3e,
        AArch64         = 0xb7,
        RISCV           = 0xf3
};

struct [[gnu::packed]] ELFHeader {
        lib::u8  magic[4];      // 0x7f 'E' 'L' 'F'
        lib::u8  machine;       // 1 = 32-bit, 2 = 64-bit
        lib::u8  endian;        // 1 = little, 2 = big
        lib::u8  hdr_version;
        lib::u8  abi;
        lib::u64 __padding;
        lib::u16 type;
        lib::u16 isa;
        lib::u32 elf_version;
        lib::u64 entry_offset;
        lib::u64 prog_hdr_table_offset;
        lib::u64 prog_sect_table_offset;
        lib::u32 flags;
        lib::u16 hdr_size;
        lib::u16 prog_hdr_table_entry_size;
        lib::u16 prog_hdr_table_entry_count;
        lib::u16 prog_hdr_sect_entry_size;
        lib::u16 prog_hdr_sect_entry_count;
        lib::u16 hdr_str_table;
};

struct [[gnu::packed]] SectionHeader {
        lib::u32 name;
        lib::u32 type;
        lib::u64 flags;
        lib::u64 addr;
        lib::u64 off;
        lib::u64 size;
        lib::u32 link;
        lib::u32 info;
        lib::u64 addralign;
        lib::u64 entsize;
};

struct [[gnu::packed]] ProgramHeader {
        lib::u32 segment_type;
        lib::u32 flags;
        lib::u64 p_offset;
        lib::u64 p_vaddr;
        lib::u64 p_paddr;
        lib::u64 p_filesz;
        lib::u64 p_memsz;       // at least as big as p_filesz
        lib::u64 section_alignment;
};

// pml4t: pml4 table of the process
// hhdm: hhdm offset
int load_elf(lib::u64 *pml4t, const lib::String &path, lib::uptr hhdm);

} /* namespace kernel::proc::elf */