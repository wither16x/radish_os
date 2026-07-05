#include <kernel.hpp>
#include <lib/filesystem.hpp>
#include <lib/math.hpp>
#include <lib/memory.hpp>
#include <lib/vector.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <proc/elf.hpp>

using kernel::lib::String;
using kernel::lib::u8, kernel::lib::u16, kernel::lib::u32, kernel::lib::u64, kernel::lib::uptr, kernel::lib::usize;
using kernel::lib::Vector;
using kernel::lib::getfilesz, kernel::lib::read;
using kernel::lib::memcpy, kernel::lib::memset;
using kernel::lib::min;

namespace kernel::proc::elf {

namespace {

/// Supported program header types.
enum PHDRType : elf64_word {
        PHDRTypeLoad            = 1
};

/// Check if an ELF is valid or not.
int elf_check(ELF64Ehdr *hdr)
{
        if (hdr->e_ident[ELF64Magic::EI_I0] != ELF64Magic::EI_BYTE0
        || hdr->e_ident[ELF64Magic::EI_I1] != ELF64Magic::EI_BYTE1
        || hdr->e_ident[ELF64Magic::EI_I2] != ELF64Magic::EI_BYTE2
        || hdr->e_ident[ELF64Magic::EI_I3] != ELF64Magic::EI_BYTE3
        ) {
                return -1; // invalid magic
        }

        if (hdr->e_type != ELF64Type::ET_EXEC)
                return -2; // invalid ELF type

        if (hdr->e_version != 1)
                return -3; // unsupported ELF version

        return 0;
}

} /* anonymous namespace */

// --------------------------------------------------
int load_elf(u64 *pml4t, const String &path, uptr *addr)
{
        uptr hhdm = get_kernel_hhdm_offset();

        // read the file
        Vector<u8> buf;
        usize size = 0;
        getfilesz(path, &size);
        buf.resize(size);
        read(path, reinterpret_cast<char *>(buf.get_data()), size);

        // parse the file
        ELF64Ehdr *hdr = reinterpret_cast<ELF64Ehdr *>(buf.get_data());
        int is_file_valid = elf_check(hdr);
        if (is_file_valid != 0)
                return -1;

        uptr phdr_offset = reinterpret_cast<uptr>(hdr) + hdr->e_phoff;

        for (elf64_half i = 0; i < hdr->e_phnum; i++) {
                ELF64Phdr *phdr = reinterpret_cast<ELF64Phdr *>(phdr_offset + i * hdr->e_phentsize);        
                if (phdr->p_type != PHDRType::PHDRTypeLoad)
                        continue;

                usize pages = (phdr->p_memsz + mem::vmm::PAGE_BYTES - 1) / mem::vmm::PAGE_BYTES;
                for (usize j = 0; j < pages; j++) {
                        // since the buffer containing the data is allocated on the heap, it cannot
                        // be executed as all heap pages are NX, so we copy the data from these pages
                        // to new RX pages
                        uptr frame = mem::pmm::allocate_frame();
                        uptr vaddr = phdr->p_vaddr + j * mem::vmm::PAGE_BYTES;

                        u32 pflags = phdr->p_flags;
                        u64 flag;
                        if (pflags & 0x1) // PF_X
                                flag = mem::vmm::PageFlag::ReadExecUser;
                        else
                                flag = mem::vmm::PageFlag::ReadWriteUser | mem::vmm::PageFlag::NoExec;
                        mem::vmm::map_page(pml4t, vaddr, frame, flag);

                        memset(reinterpret_cast<void *>(hhdm + frame), 0, mem::vmm::PAGE_BYTES);

                        uptr src_off = phdr->p_offset + j * mem::vmm::PAGE_BYTES;
                        // we need the offset between pages as it may not be mapped otherwise
                        uptr intra_offset = phdr->p_vaddr & (mem::vmm::PAGE_BYTES - 1);

                        usize to_copy;
                        if (j * mem::vmm::PAGE_BYTES >= phdr->p_filesz)
                                to_copy = 0;
                        else
                                to_copy = min(mem::vmm::PAGE_BYTES, phdr->p_filesz - j * mem::vmm::PAGE_BYTES);
                        
                        if (to_copy > 0) {
                                memcpy(
                                        reinterpret_cast<void *>(hhdm + frame + (j == 0 ? intra_offset : 0)),
                                        buf.get_data() + src_off,
                                        to_copy
                                );
                        }
                }
        }

        // copy the entry address
        memcpy(addr, &hdr->e_entry, sizeof(*addr));

        return 0;
}
// --------------------------------------------------

} /* namespace kernel::proc::elf */