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

struct MemmapEntry {
        lib::u64 base;
        lib::u64 length;
        MemmapEntryType type;
        const char *str;
};

struct Module {
        void *address;
        lib::u64 size;
        char *path;
        char *string;
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
        MemmapEntry entries[MaxEntries];
};

struct HHDMInfo {
        lib::u64 offset;
};

struct ExecutableInfo {
        lib::u64 physical_base;
        lib::u64 virtual_base;
};

struct ModuleInfo {
        static constexpr int MaxModules = 25;

        lib::u64 count;
        Module modules[MaxModules];
};

BootloaderInfo          bootloader;
FirmwareTypeInfo        firmware_type;
MemmapInfo              memmap;
HHDMInfo                hhdm;
ExecutableInfo          executable;
ModuleInfo              modules;

BootInfo();

};

} /* namespace kernel::boot */