#include <kernel.hpp>
#include <lib/filesystem.hpp>
#include <lib/math.hpp>
#include <lib/memory.hpp>
#include <lib/vector.hpp>
#include <lib/logging.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <mem/pml4t.hpp>
#include <mem/page.hpp>
#include <proc/elf.hpp>

namespace Kiwi::Proc::Elf
{
        namespace
        {
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
        } // anonymous namespace

        int loadElf(Mem::PML4T *pml4t, const Lib::String &path, ElfInfo *info)
        {
                Lib::uptr hhdm = getKernelHhdmOffset();

                // read the file
                Lib::Vector<Lib::u8> buf;
                Lib::usize size = 0;
                getfilesz(path, &size);
                buf.resize(size);
                Lib::File *elf_file = Lib::open(path);
                read(elf_file, buf.getData(), size);
                Lib::close(elf_file);

                // parse the file
                Elf64Ehdr *hdr = reinterpret_cast<Elf64Ehdr *>(buf.getData());
                int is_file_valid = elf_check(hdr);
                if (is_file_valid != 0) {
                        Lib::Log::logger.err("elf is not valid");
                        return -1;
                }

                Lib::uptr phdr_offset = reinterpret_cast<Lib::uptr>(hdr) + hdr->e_phoff;
                Lib::uptr highest_vaddr = 0;

                for (elf64_half i = 0; i < hdr->e_phnum; i++) {
                        Elf64Phdr *phdr = reinterpret_cast<Elf64Phdr *>(phdr_offset + i * hdr->e_phentsize);        
                        if (phdr->p_type != PhdrType::PhdrTypeLoad)
                                continue;

                        Lib::uptr segment_end = phdr->p_vaddr + phdr->p_memsz;
                        if (segment_end > highest_vaddr)
                                highest_vaddr = segment_end;

                        Lib::usize pages = ((phdr->p_vaddr & (Mem::PAGE_SIZE - 1)) + phdr->p_memsz + Mem::PAGE_SIZE - 1) / Mem::PAGE_SIZE;
                        for (Lib::usize j = 0; j < pages; j++) {
                                // since the buffer containing the data is allocated on the heap, it cannot
                                // be executed as all heap pages are NX, so we copy the data from these pages
                                // to new RX pages
                                Lib::uptr frame = Mem::Pmm::allocateFrame();
                                Lib::uptr vaddr = (phdr->p_vaddr & ~(Mem::PAGE_SIZE - 1)) + j * Mem::PAGE_SIZE;

                                Lib::u32 pflags = phdr->p_flags;
                                Lib::u64 flag;
                                if (pflags & 0x1) // PF_X
                                        flag = Mem::PageFlag::ReadExecUser;
                                else
                                        flag = Mem::PageFlag::ReadWriteUser | Mem::PageFlag::NoExec;
                                pml4t->mapPage(vaddr, frame, flag);

                                Lib::memset(reinterpret_cast<void *>(hhdm + frame), 0, Mem::PAGE_SIZE);

                                // we need the offset between pages as it may not be mapped otherwise
                                Lib::uptr intra_offset = phdr->p_vaddr & (Mem::PAGE_SIZE - 1);
                                Lib::uptr src_off = phdr->p_offset + j * Mem::PAGE_SIZE - (j == 0 ? 0 : intra_offset);

                                Lib::usize to_copy = 0;
                                if (j == 0) {
                                        to_copy = Lib::min<Lib::usize>(Mem::PAGE_SIZE - intra_offset, phdr->p_filesz);
                                } else {
                                        Lib::uptr rel_off = j * Mem::PAGE_SIZE - intra_offset;
                                        if (rel_off < phdr->p_filesz)
                                                to_copy = Lib::min<Lib::usize>(Mem::PAGE_SIZE, phdr->p_filesz - rel_off);                                
                                }

                                if (to_copy > 0) {
                                        Lib::memcpy(
                                                reinterpret_cast<void *>(hhdm + frame + (j == 0 ? intra_offset : 0)),
                                                buf.getData() + src_off,
                                                to_copy
                                        );
                                }
                        }
                }

                // copy the entry address
                Lib::memcpy(&info->address, &hdr->e_entry, sizeof(info->address));
                // the highest virtual address is at the end of the executable
                info->highest_vaddr = highest_vaddr;

                return 0;
        }
} // namespace Kiwi::Proc::Elf