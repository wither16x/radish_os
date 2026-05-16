#pragma once

namespace kernel::boot {

// To avoid the kernel to depend on the boot protocol everywhere,
// it has its own structs filled using the informations provided
// by the boot protocol.
struct boot_info {

struct bootloader_info {
        const char *name;
        const char *version;
};

bootloader_info bootloader;

boot_info();
};

} /* namespace kernel::boot */