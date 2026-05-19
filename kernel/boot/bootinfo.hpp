#pragma once

#include <lib/typing.hpp>

namespace kernel::boot {

enum class FirmwareType : int {
        X86BIOS,
        EFI32,
        EFI64,
        SBI
};

enum class MemmapEntryType : int {
        Usable,
        Reserved,
        ACPIReclaimable,
        ACPINVS,
        BadMemory,
        BootloaderReclaimable,
        ExecutableAndModules,
        Framebuffer,
        ReservedMapped
};

struct memmap_entry {
        lib::u64 base;
        lib::u64 length;
        MemmapEntryType type;
        const char *str;
};

// To avoid the kernel to depend on the boot protocol everywhere,
// it has its own structs filled using the informations provided
// by the boot protocol.
struct BootInfo {

struct BootloaderInfo {
        const char *name;
        const char *version;
};

struct FirmwareTypeInfo {
        FirmwareType type;
        const char *str;
};

struct MemmapInfo {
        static constexpr int MaxEntries = 64;

        lib::u64 entry_count;
        memmap_entry entries[MaxEntries];
};

BootloaderInfo         bootloader;
FirmwareTypeInfo      firmware_type;
MemmapInfo             memmap;

BootInfo();

};

} /* namespace kernel::boot */