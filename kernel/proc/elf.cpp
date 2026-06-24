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

} /* anonymous namespace */

int load_elf(u64 *pml4t, const String &path, uptr hhdm, uptr *addr)
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

        logger.debug("parsing program headers");
        logger.debug("------------------------------");
        uptr phdr_offset = reinterpret_cast<uptr>(hdr) + hdr->e_phoff;
        uptr required_size = 0;
        for (elf64_half i = 0; i < hdr->e_phnum; i++) {
                ELF64Phdr *phdr = reinterpret_cast<ELF64Phdr *>(phdr_offset + i * hdr->e_phentsize);        
                required_size += phdr->p_memsz;
        }

        usize required_pages = (required_size + mem::vmm::PAGE_BYTES - 1) / mem::vmm::PAGE_BYTES;
        logger.debug("pages needed: %u", required_pages);

        // since the buffer is allocated on the heap, the pages it is on are
        // not executable, so we need to copy its content to a new executable
        // page
        for (usize i = 1; i <= required_pages; i++) {
                uptr frame = mem::pmm::allocate_frame();
                mem::vmm::map_page(pml4t, hdr->e_entry, frame, mem::vmm::PageFlag::ReadExec);
                memcpy(reinterpret_cast<void *>(hhdm + frame), buf.get_data() + mem::vmm::PAGE_BYTES * i, mem::vmm::PAGE_BYTES);
        }

        memcpy(addr, &hdr->e_entry, sizeof(*addr));

        return 0;
}

} /* namespace kernel::proc::elf */