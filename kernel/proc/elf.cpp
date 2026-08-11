#include <kernel.hpp>
#include <lib/filesystem.hpp>
#include <lib/math.hpp>
#include <lib/memory.hpp>
#include <lib/vector.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <mem/pml4t.hpp>
#include <mem/page.hpp>
#include <proc/elf.hpp>

#include <lib/logging.hpp>
using kernel::lib::log::logger;

using kernel::lib::String;
using kernel::lib::u8, kernel::lib::u16, kernel::lib::u32, kernel::lib::u64, kernel::lib::uptr, kernel::lib::usize;
using kernel::lib::Vector;
using kernel::lib::getfilesz;
using kernel::lib::memcpy, kernel::lib::memset;
using kernel::lib::min;

namespace kernel::proc::elf {

namespace {

/// Supported program header types.
enum PhdrType : elf64_word {
        PhdrTypeLoad            = 1
};

/// Check if an ELF is valid or not.
int elf_check(Elf64Ehdr *hdr)
{
        if (hdr->e_ident[Elf64Magic::EI_I0] != Elf64Magic::EI_BYTE0
        or hdr->e_ident[Elf64Magic::EI_I1] != Elf64Magic::EI_BYTE1
        or hdr->e_ident[Elf64Magic::EI_I2] != Elf64Magic::EI_BYTE2
        or hdr->e_ident[Elf64Magic::EI_I3] != Elf64Magic::EI_BYTE3
        ) {
                return -1; // invalid magic
        }

        if (hdr->e_type != Elf64Type::ET_EXEC)
                return -2; // invalid ELF type

        if (hdr->e_version != 1)
                return -3; // unsupported ELF version

        return 0;
}

} /* anonymous namespace */

// --------------------------------------------------
int load_elf(mem::PML4T *pml4t, const String &path, ElfInfo *info)
{
        uptr hhdm = get_kernel_hhdm_offset();

        // read the file
        Vector<u8> buf;
        usize size = 0;
        getfilesz(path, &size);
        buf.resize(size);

        lib::File *elf_file = lib::open(path);
        read(elf_file, buf.get_data(), size);
        lib::close(elf_file);

        // parse the file
        Elf64Ehdr *hdr = reinterpret_cast<Elf64Ehdr *>(buf.get_data());
        int is_file_valid = elf_check(hdr);
        if (is_file_valid != 0) {
                logger.err("elf is not valid");
                return -1;
        }

        uptr phdr_offset = reinterpret_cast<uptr>(hdr) + hdr->e_phoff;
        uptr highest_vaddr = 0;

        for (elf64_half i = 0; i < hdr->e_phnum; i++) {
                Elf64Phdr *phdr = reinterpret_cast<Elf64Phdr *>(phdr_offset + i * hdr->e_phentsize);        
                if (phdr->p_type != PhdrType::PhdrTypeLoad)
                        continue;

                uptr segment_end = phdr->p_vaddr + phdr->p_memsz;
                if (segment_end > highest_vaddr)
                        highest_vaddr = segment_end;

                usize pages = ((phdr->p_vaddr & (mem::PAGE_SIZE - 1)) + phdr->p_memsz + mem::PAGE_SIZE - 1) / mem::PAGE_SIZE;
                for (usize j = 0; j < pages; j++) {
                        // since the buffer containing the data is allocated on the heap, it cannot
                        // be executed as all heap pages are NX, so we copy the data from these pages
                        // to new RX pages
                        uptr frame = mem::pmm::allocate_frame();
                        uptr vaddr = (phdr->p_vaddr & ~(mem::PAGE_SIZE - 1)) + j * mem::PAGE_SIZE;

                        u32 pflags = phdr->p_flags;
                        u64 flag;
                        if (pflags & 0x1) // PF_X
                                flag = mem::PageFlag::ReadExecUser;
                        else
                                flag = mem::PageFlag::ReadWriteUser | mem::PageFlag::NoExec;
                        pml4t->map_page(vaddr, frame, flag);

                        memset(reinterpret_cast<void *>(hhdm + frame), 0, mem::PAGE_SIZE);

                        // we need the offset between pages as it may not be mapped otherwise
                        uptr intra_offset = phdr->p_vaddr & (mem::PAGE_SIZE - 1);
                        uptr src_off = phdr->p_offset + j * mem::PAGE_SIZE - (j == 0 ? 0 : intra_offset);

                        usize to_copy = 0;
                        if (j == 0) {
                                to_copy = min<usize>(mem::PAGE_SIZE - intra_offset, phdr->p_filesz);
                        } else {
                                uptr rel_off = j * mem::PAGE_SIZE - intra_offset;
                                if (rel_off < phdr->p_filesz)
                                        to_copy = min<usize>(mem::PAGE_SIZE, phdr->p_filesz - rel_off);                                
                        }

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
        memcpy(&info->address, &hdr->e_entry, sizeof(info->address));
        // the highest virtual address is at the end of the executable
        info->highest_vaddr = highest_vaddr;

        return 0;
}
// --------------------------------------------------

} /* namespace kernel::proc::elf */