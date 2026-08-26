#pragma once

#include <lib/typing.hpp>

namespace Kiwi::Boot
{
        /// Enumeration of the types of firmware supported
        /// byte the Limine Boot Protocol.
        enum class FirmwareType
        {
                X86Bios,
                Efi32,
                Efi64,
                Sbi
        };

        /// Enumeration of the types of memory map entries
        /// supported by the Limine Boot Protocol.
        enum class MemmapEntryType
        {
                Usable,
                Reserved,
                AcpiReclaimable,
                AcpiNvs,
                BadMemory,
                BootloaderReclaimable,
                ExecutableAndModules,
                Framebuffer,
                ReservedMapped
        };

        /// Protocol-independent representation of a single memory
        /// map entry.
        struct MemmapEntry
        {
                Lib::u64 base;
                Lib::u64 length;
                MemmapEntryType type;
                const char *str;
        };

        /// Protocol-independent representation of a single module.
        struct Module
        {
                void *address;
                Lib::u64 size;
                char *path;
                char *string;
        };

        // To avoid the kernel to depend on the boot protocol everywhere,
        // it has its own structs filled using the informations provided
        // by the boot protocol.

        /// General protocol-independent structure wrapping every
        /// information that the kernel can receive from the bootloader.
        struct BootInfo
        {
                /// Informations about the bootloader.
                struct BootloaderInfo
                {
                        const char *name;
                        const char *version;
                };

                /// Type of firmware the machine uses.
                struct FirmwareTypeInfo
                {
                        FirmwareType type;
                        const char *str;
                };

                /// Informations on the memory map. A limit of 64 entries should
                /// be enough.
                struct MemmapInfo
                {
                        static constexpr int MAX_ENTRIES = 64;

                        Lib::u64 entry_count;
                        MemmapEntry entries[MAX_ENTRIES];
                };

                /// Informations about the HHDM (Higher-Half Direct Mapping).
                struct HhdmInfo
                {
                        Lib::u64 offset;
                };

                /// Informations about the kernel, treated as the "executable"
                /// by the bootloader.
                struct ExecutableInfo
                {
                        Lib::u64 physical_base;
                        Lib::u64 virtual_base;
                };

                /// Informations about modules, which are files loaded
                /// alongside the kernel. A limit of 25 modules should be
                /// enough.
                struct ModuleInfo
                {
                        static constexpr int MAX_MODULES = 25;

                        Lib::u64 count;
                        Module modules[MAX_MODULES];
                };

                /// Informations about the framebuffer.
                struct FramebufferInfo
                {
                        void *address;
                        Lib::u64 pitch;
                        Lib::u64 width;
                        Lib::u64 height;
                        Lib::u16 bpp;           // bits per pixel
                        Lib::u8 red_mask_size;
                        Lib::u8 red_mask_shift;
                        Lib::u8 green_mask_size;
                        Lib::u8 green_mask_shift;
                        Lib::u8 blue_mask_size;
                        Lib::u8 blue_mask_shift;
                };

                BootloaderInfo          bootloader;
                FirmwareTypeInfo        firmware_type;
                MemmapInfo              memmap;
                HhdmInfo                hhdm;
                ExecutableInfo          executable;
                ModuleInfo              modules;
                FramebufferInfo         framebuffer;

                /// Automatically initialize boot informations.
                BootInfo();
        };
} /* namespace Kiwi::Boot */