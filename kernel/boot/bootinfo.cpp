#include <boot/bootinfo.hpp>
#include <limine.h>

namespace kernel::boot {

namespace {

limine_bootloader_info_request l_bootloader_info = {
        .id = LIMINE_BOOTLOADER_INFO_REQUEST_ID,
        .revision = 0,
        .response = nullptr
};

} /* anonymous namespace */

boot_info::boot_info()
{
        this->bootloader.name = l_bootloader_info.response->name;
        this->bootloader.version = l_bootloader_info.response->version;
}

} /* namespace kernel::boot */