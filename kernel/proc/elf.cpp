#include <kernel.hpp>
#include <lib/filesystem.hpp>
#include <lib/memory.hpp>
#include <lib/vector.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <proc/elf.hpp>

#include <lib/logging.hpp>
using kernel::lib::log::logger;

using kernel::lib::String;
using kernel::lib::u8, kernel::lib::u16, kernel::lib::u64, kernel::lib::uptr, kernel::lib::usize;
using kernel::lib::Vector;
using kernel::lib::getfilesz, kernel::lib::read_file;
using kernel::lib::memcpy;

namespace kernel::proc::elf {

namespace {

int elf_check(ELFHeader *hdr)
{
        if (hdr->magic[0] != 0x7f
            || hdr->magic[1] != 'E'
            || hdr->magic[2] != 'L'
            || hdr->magic[3] != 'F'
        ) {
                return -1; // invalid magic
        }

        if (hdr->machine != MachineType::Bit64)
                return -2; // unsupported machine

        if (hdr->hdr_version != ELFVersion::Version1)
                return -3; // unsupported ELF version

        if (hdr->type != ELFType::Executable)
                return -4; // umsupported ELF type

        return 0;
}

inline SectionHeader *get_section_header(ELFHeader *hdr)
{
        return reinterpret_cast<SectionHeader *>(
                reinterpret_cast<u64>(hdr) + hdr->prog_sect_table_offset
        );
}

inline SectionHeader *get_section(ELFHeader *hdr, u64 idx)
{
        return &get_section_header(hdr)[idx];
}

inline char *get_string_table(ELFHeader *hdr)
{
        return reinterpret_cast<char *>(hdr) + get_section(hdr, hdr->hdr_str_table)->off;
}

inline char *lookup_string(ELFHeader *hdr, u64 offset)
{
        char *string_table = get_string_table(hdr);
        return string_table + offset;
}

} /* anonymous namespace */

int load_elf(u64 *pml4t, const String &path, uptr hhdm)
{
        // read the file
        Vector<u8> buf;
        usize size = 0;

        getfilesz(path, &size);
        buf.resize(size);

        read_file(path, reinterpret_cast<char *>(buf.get_data()), size);

        ELFHeader *hdr = reinterpret_cast<ELFHeader *>(buf.get_data());
        
        int is_file_valid = elf_check(hdr);
        if (is_file_valid != 0)
                return -1;

        logger.debug("machine: 0x%x", hdr->machine);
        logger.debug("endian: 0x%x", hdr->endian);
        logger.debug("hdr version: 0x%x", hdr->hdr_version);
        logger.debug("ABI: 0x%x", hdr->abi);
        logger.debug("type: 0x%x", hdr->type);
        logger.debug("ISA: 0x%x", hdr->isa);
        logger.debug("version: 0x%x", hdr->elf_version);
        logger.debug("entry: 0x%x", hdr->entry_offset);
        logger.debug("prog hdr table off: 0x%x", hdr->prog_hdr_table_offset);
        logger.debug("prog sect table off: 0x%x", hdr->prog_sect_table_offset);
        logger.debug("flags: 0x%x", hdr->flags);
        logger.debug("size: 0x%x", hdr->hdr_size);
        logger.debug("table entry size: 0x%x", hdr->prog_hdr_table_entry_size);
        logger.debug("table entry count: 0x%x", hdr->prog_hdr_table_entry_count);
        logger.debug("section entry size: 0x%x", hdr->prog_hdr_sect_entry_size);
        logger.debug("section entry count: 0x%x", hdr->prog_hdr_sect_entry_count);
        logger.debug("string table: 0x%x", hdr->hdr_str_table);

        char *section_name = lookup_string(hdr, 0x1000);
        logger.debug("section %s", section_name);

        // since the buffer is allocated on the heap, the pages it is on are
        // not executable, so we need to copy its content to a new executable
        // page
        uptr frame = mem::pmm::allocate_frame();
        mem::vmm::map_page(pml4t, hdr->entry_offset, frame, mem::vmm::PageFlag::ReadExec);
        memcpy(reinterpret_cast<void *>(hhdm + frame), buf.get_data(), buf.size());

        return 0;
}

} /* namespace kernel::proc::elf */