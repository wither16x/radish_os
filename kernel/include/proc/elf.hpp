#pragma once

#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <mem/pml4t.hpp>

namespace kernel::proc::elf {

constexpr int EI_NIDENT = 16;

/// Magic bytes of an ELF file and their indexes.
enum ELF64Magic : unsigned char {
        EI_I0              = 0,
        EI_I1,
        EI_I2,
        EI_I3,
        EI_BYTE0           = 0x7f,
        EI_BYTE1           = 'E',
        EI_BYTE2           = 'L',
        EI_BYTE3           = 'F'
};

// ELF64 specific types

using elf64_half                = lib::u16;
using elf64_word                = lib::u32;
using elf64_sword               = lib::i32;
using elf64_xword               = lib::u64;
using elf64_sxword              = lib::i64;
using elf64_addr                = lib::u64;
using elf64_off                 = lib::u64;
using elf64_section             = lib::u16;
using elf64_versym              = elf64_half;

/// Enumeration of supported ELF types.
enum ELF64Type : elf64_half {
        ET_EXEC           = 2
};

/// Handled symbol states.
enum SHN : int {
        SHN_UNDEF
};

// eh = ELF header
// ph = program header
// sh = section header

/// Header of an ELF.
struct ELF64Ehdr {
        unsigned char   e_ident[EI_NIDENT];
        elf64_half      e_type;
        elf64_half      e_machine;
        elf64_word      e_version;
        elf64_addr      e_entry;
        elf64_off       e_phoff;
        elf64_off       e_shoff;
        elf64_word      e_flags;
        elf64_half      e_ehsize;
        elf64_half      e_phentsize;
        elf64_half      e_phnum;
        elf64_half      e_shentsize;
        elf64_half      e_shnum;
        elf64_half      e_shstrndx;
};

/// Header of a section.
struct ELF64Shdr {
        elf64_word      sh_name;
        elf64_word      sh_type;
        elf64_xword     sh_flags;
        elf64_addr      sh_addr;
        elf64_off       sh_offset;
        elf64_xword     sh_size;
        elf64_word      sh_link;
        elf64_word      sh_info;
        elf64_xword     sh_addralign;
        elf64_xword     sh_entsize;
};

/// Header of a program.
struct ELF64Phdr {
        elf64_word      p_type;
        elf64_word      p_flags;
        elf64_off       p_offset;
        elf64_addr      p_vaddr;
        elf64_addr      p_paddr;
        elf64_xword     p_filesz;
        elf64_xword     p_memsz;
        elf64_xword     p_align;
};

/// Representation of a symbol.
struct ELF64Sym {
        elf64_word      st_name;
        unsigned char   st_info;
        unsigned char   st_other;
        elf64_section   st_shndx;
        elf64_addr      st_value;
        elf64_xword     st_size;
};

/// Load an ELF and map it in higher-half in a process'
/// PML4 table.
int load_elf(mem::PML4T *pml4t, const lib::String &path, lib::uptr *addr);

} /* namespace kernel::proc::elf */