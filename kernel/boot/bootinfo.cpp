#include <boot/bootinfo.hpp>
#include <limine.h>

namespace kernel::boot {

namespace {

limine_bootloader_info_request l_bootloader_info = {
        .id = LIMINE_BOOTLOADER_INFO_REQUEST_ID,
        .revision = 0,
        .response = nullptr
};

limine_firmware_type_request l_firmware_type = {
        .id = LIMINE_FIRMWARE_TYPE_REQUEST_ID,
        .revision = 0,
        .response = nullptr
};

const char *firmware_type_str_id[] = {
        "x86 BIOS",
        "EFI 32",
        "EFI 64",
        "SBI"
};

} /* anonymous namespace */

boot_info::boot_info()
{
        this->bootloader.name           = l_bootloader_info.response->name;
        this->bootloader.version        = l_bootloader_info.response->version;

        this->firmware_type.type        = static_cast<FirmwareType>(l_firmware_type.response->firmware_type);
        this->firmware_type.str         = firmware_type_str_id[static_cast<int>(this->firmware_type.type)];
}

} /* namespace kernel::boot */