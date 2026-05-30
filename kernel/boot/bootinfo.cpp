#include <boot/bootinfo.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>
#include <limine.h>

using kernel::lib::log::logger;
using kernel::lib::u64;

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

limine_memmap_request l_memmap_info = {
        .id = LIMINE_MEMMAP_REQUEST_ID,
        .revision = 0,
        .response = nullptr
};

limine_hhdm_request l_hhdm_info = {
        .id = LIMINE_HHDM_REQUEST_ID,
        .revision = 0,
        .response = nullptr      
};

limine_executable_address_request l_executable_info = {
        .id = LIMINE_EXECUTABLE_ADDRESS_REQUEST_ID,
        .revision = 0,
        .response = nullptr
};

const char *firmware_type_str[] = {
        "x86 BIOS",
        "EFI 32",
        "EFI 64",
        "SBI"
};

const char *memmap_type_str[] = {
        "usable",
        "reserved",
        "ACPI reclaimable",
        "ACPI NVS",
        "bad memory",
        "bootloader reclaimable",
        "executable and modules",
        "framebuffer",
        "reserved mapped"
};

} /* anonymous namespace */

BootInfo::BootInfo()
{
        logger.info("recovering boot info");

        this->bootloader.name           = l_bootloader_info.response->name;
        this->bootloader.version        = l_bootloader_info.response->version;

        this->firmware_type.type        = static_cast<FirmwareType>(l_firmware_type.response->firmware_type);
        this->firmware_type.str         = firmware_type_str[static_cast<int>(this->firmware_type.type)];

        this->memmap.entry_count        = l_memmap_info.response->entry_count;
        for (u64 i = 0; i < this->memmap.entry_count; i++) {
                limine_memmap_entry **l_entries = l_memmap_info.response->entries;
                memmap_entry e = {
                        .base   = l_entries[i]->base,
                        .length = l_entries[i]->length,
                        .type   = static_cast<MemmapEntryType>(l_entries[i]->type),
                        .str    = memmap_type_str[l_entries[i]->type]
                };
                this->memmap.entries[i] = e;
        }

        this->hhdm.offset               = l_hhdm_info.response->offset;

        this->executable.physical_base  = l_executable_info.response->physical_base;
        this->executable.virtual_base   = l_executable_info.response->virtual_base;

        logger.ok("recovered boot info");
}

} /* namespace kernel::boot */