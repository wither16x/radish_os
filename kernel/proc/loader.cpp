#include <kernel.hpp>
#include <lib/filesystem.hpp>
#include <lib/memory.hpp>
#include <lib/vector.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <proc/loader.hpp>

using kernel::lib::String;
using kernel::lib::u8, kernel::lib::u64, kernel::lib::uptr, kernel::lib::usize;
using kernel::lib::Vector;
using kernel::lib::getfilesz, kernel::lib::read_file;
using kernel::lib::memcpy;

namespace kernel::proc {

void load_program(u64 *pml4t, const String &path, uptr address, uptr hhdm)
{
        // read the file
        Vector<u8> buf;
        usize size = 0;

        getfilesz(path, &size);
        buf.resize(size);

        read_file(path, reinterpret_cast<char *>(buf.get_data()), size);

        // since the buffer is allocated on the heap, the pages it is on are
        // not executable, so we need to copy its content to a new executable
        // page
        uptr frame = mem::pmm::allocate_frame();
        mem::vmm::map_page(pml4t, address, frame, mem::vmm::PageFlag::ReadExec);
        memcpy(reinterpret_cast<void *>(hhdm + frame), buf.get_data(), buf.size());
}

} /* namespace kernel::proc */