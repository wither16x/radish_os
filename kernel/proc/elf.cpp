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

int elf_check(ELF64Ehdr *hdr)
{
        if (hdr->e_ident[ELF64Magic::Index0] != ELF64Magic::Byte0
        || hdr->e_ident[ELF64Magic::Index1] != ELF64Magic::Byte1
        || hdr->e_ident[ELF64Magic::Index2] != ELF64Magic::Byte2
        || hdr->e_ident[ELF64Magic::Index3] != ELF64Magic::Byte3
        ) {
                return -1; // invalid magic
        }

        if (hdr->e_type != ELF64Type::ELF64TypeExec)
                return -2; // invalid ELF type

        if (hdr->e_version != 1)
                return -3; // unsupported ELF version

        return 0;
}

inline ELF64Shdr *get_section_header(ELF64Ehdr *hdr)
{
        return reinterpret_cast<ELF64Shdr *>(
                reinterpret_cast<u64>(hdr) + hdr->e_shoff
        );
}

inline ELF64Shdr *get_section(ELF64Ehdr *hdr, u64 idx)
{
        return &get_section_header(hdr)[idx];
}

inline char *get_string_table(ELF64Ehdr *hdr)
{
        if (hdr->e_shstrndx == SHN::SHNUndef)
                return nullptr;
        return reinterpret_cast<char *>(hdr) + get_section(hdr, hdr->e_shstrndx)->sh_offset;
}

inline char *lookup_string(ELF64Ehdr *hdr, u64 offset)
{
        char *string_table = get_string_table(hdr);
        if (!string_table)
                return nullptr;
        return string_table + offset;
}

} /* anonymous namespace */

int load_elf(u64 *pml4t, const String &path, uptr hhdm)
{
        logger.debug("loading %s", path.raw());
        logger.debug("------------------------------");

        // read the file
        Vector<u8> buf;
        usize size = 0;

        getfilesz(path, &size);
        buf.resize(size);

        read_file(path, reinterpret_cast<char *>(buf.get_data()), size);

        // parse the file
        ELF64Ehdr *hdr = reinterpret_cast<ELF64Ehdr *>(buf.get_data());
        
        int is_file_valid = elf_check(hdr);
        if (is_file_valid != 0)
                return -1;

        Vector<ELF64Phdr *> phdrs;
        phdrs.resize(hdr->e_phnum);

        logger.debug("parsing program headers");
        logger.debug("------------------------------");

        uptr phdr_offset = reinterpret_cast<uptr>(hdr) + hdr->e_phoff;
        logger.debug("1st PHDR offset = 0x%x", phdr_offset);
        for (elf64_half i = 0; i < hdr->e_phnum; i++) {
                ELF64Phdr *phdr = reinterpret_cast<ELF64Phdr *>(phdr_offset + i * hdr->e_phentsize);
                logger.debug("phdr#%u.vaddr = 0x%x", i, phdr->p_vaddr);
        
                phdrs.push_back(phdr);
        }

        // since the buffer is allocated on the heap, the pages it is on are
        // not executable, so we need to copy its content to a new executable
        // page
        uptr frame = mem::pmm::allocate_frame();
        mem::vmm::map_page(pml4t, hdr->e_entry, frame, mem::vmm::PageFlag::ReadExec);
        memcpy(reinterpret_cast<void *>(hhdm + frame), buf.get_data(), buf.size());

        return 0;
}

} /* namespace kernel::proc::elf */