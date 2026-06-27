#pragma once

#include <lib/typing.hpp>

namespace kernel::boot {

/// Enumeration of the types of firmware supported
/// byte the Limine Boot Protocol.
enum class FirmwareType : int {
        X86BIOS,
        EFI32,
        EFI64,
        SBI
};

/// Enumeration of the types of memory map entries
/// supported by the Limine Boot Protocol.
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

/// Protocol-independent representation of a single memory
/// map entry.
struct MemmapEntry {
        lib::u64 base;
        lib::u64 length;
        MemmapEntryType type;
        const char *str;
};

/// Protocol-independent representation of a single module.
struct Module {
        void *address;
        lib::u64 size;
        char *path;
        char *string;
};

// To avoid the kernel to depend on the boot protocol everywhere,
// it has its own structs filled using the informations provided
// by the boot protocol.

/// General protocol-independent structure wrapping every
/// information that the kernel can receive from the bootloader.
struct BootInfo {

/// Informations about the bootloader.
struct BootloaderInfo {
        const char *name;
        const char *version;
};

/// Type of firmware the machine uses.
struct FirmwareTypeInfo {
        FirmwareType type;
        const char *str;
};

/// Informations on the memory map. A limit of 64 entries should
/// be enough.
struct MemmapInfo {
        static constexpr int MAX_ENTRIES = 64;

        lib::u64 entry_count;
        MemmapEntry entries[MAX_ENTRIES];
};

/// Informations about the HHDM (Higher-Half Direct Mapping).
struct HHDMInfo {
        lib::u64 offset;
};

/// Informations about the kernel, treated as the "executable"
/// by the bootloader.
struct ExecutableInfo {
        lib::u64 physical_base;
        lib::u64 virtual_base;
};

/// Informations about modules, which are files loaded
/// alongside the kernel. A limit of 25 modules should be
/// enough.
struct ModuleInfo {
        static constexpr int MAX_MODULES = 25;

        lib::u64 count;
        Module modules[MAX_MODULES];
};

/// Informations about the framebuffer.
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

/// Automatically initialize boot informations.
BootInfo();

};

} /* namespace kernel::boot */