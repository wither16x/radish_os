#pragma once

namespace kernel::boot {

enum class FirmwareType : int {
        X86BIOS,
        EFI32,
        EFI64,
        SBI
};

// To avoid the kernel to depend on the boot protocol everywhere,
// it has its own structs filled using the informations provided
// by the boot protocol.
struct boot_info {

struct bootloader_info {
        const char *name;
        const char *version;
};

struct firmware_type_info {
        FirmwareType type;
        const char *str;
};

bootloader_info         bootloader;
firmware_type_info      firmware_type;

boot_info();
};

} /* namespace kernel::boot */