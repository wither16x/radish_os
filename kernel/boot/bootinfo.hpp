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
        static constexpr int MAX_ENTRIES = 64;

        lib::u64 entry_count;
        MemmapEntry entries[MAX_ENTRIES];
};

struct HHDMInfo {
        lib::u64 offset;
};

struct ExecutableInfo {
        lib::u64 physical_base;
        lib::u64 virtual_base;
};

struct ModuleInfo {
        static constexpr int MAX_MODULES = 25;

        lib::u64 count;
        Module modules[MAX_MODULES];
};

struct FramebufferInfo {
        void *address;
        lib::u64 pitch;
        lib::u64 width;
        lib::u64 height;
        lib::u16 bpp;           // bits per pixel
        lib::u8 red_mask_size;
        lib::u8 red_mask_shift;
        lib::u8 green_mask_size;
        lib::u8 green_mask_shift;
        lib::u8 blue_mask_size;
        lib::u8 blue_mask_shift;
};

BootloaderInfo          bootloader;
FirmwareTypeInfo        firmware_type;
MemmapInfo              memmap;
HHDMInfo                hhdm;
ExecutableInfo          executable;
ModuleInfo              modules;
FramebufferInfo         framebuffer;

BootInfo();

};

} /* namespace kernel::boot */