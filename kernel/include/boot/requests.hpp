#pragma once

#include <lib/string.hpp>
#include <lib/array.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Boot
{
        enum class RequestType
        {
                BootloaderInfo,
                FirmwareType,
                Memmap,
                Hhdm,
                ExecutableAddress,
                Module,
                Framebuffer
        };

        struct Request
        {};

        struct BootloaderInfoRequest : Request
        {
                static constexpr RequestType kind = RequestType::BootloaderInfo;

                static constexpr Lib::usize NAME_SIZE = 25;
                static constexpr Lib::usize VERSION_SIZE = 25;

                Lib::String<NAME_SIZE> name;
                Lib::String<VERSION_SIZE> version;
        };

        enum class FirmwareType
        {
                X86Bios,
                Efi32,
                Efi64,
                Sbi
        };

        struct FirmwareTypeRequest : Request
        {
                static constexpr RequestType kind = RequestType::FirmwareType;

                FirmwareType type;
        };

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

        struct MemmapEntry
        {
                Lib::uptr base;
                Lib::uptr length;
                MemmapEntryType type;
        };

        struct MemmapRequest : Request
        {
                static constexpr RequestType kind = RequestType::Memmap;

                static constexpr Lib::usize MAX_ENTRIES = 64;

                Lib::usize entry_count;
                Lib::Array<MemmapEntry, MAX_ENTRIES> entries;
        };

        struct HhdmRequest : Request
        {
                static constexpr RequestType kind = RequestType::Hhdm;

                Lib::uptr offset;
        };

        struct ExecutableAddressRequest : Request
        {
                static constexpr RequestType kind = RequestType::ExecutableAddress;

                Lib::uptr phys_base;
                Lib::uptr virt_base;
        };

        struct Module
        {
                static constexpr Lib::usize PATH_LENGTH = 255;

                void *address;
                Lib::usize size;
                Lib::String<PATH_LENGTH> path;
        };

        struct ModuleRequest : Request
        {
                static constexpr RequestType kind = RequestType::Module;

                static constexpr Lib::usize MAX_MODULES = 25;

                Lib::usize module_count;
                Lib::Array<Module, MAX_MODULES> modules;
        };

        struct FramebufferRequest : Request
        {
                static constexpr RequestType kind = RequestType::Framebuffer;

                void *address;
                Lib::u64 pitch;
                Lib::u64 width;
                Lib::u64 height;
                Lib::u16 bpp;
                Lib::u8 red_mask_size;
                Lib::u8 red_mask_shift;
                Lib::u8 green_mask_size;
                Lib::u8 green_mask_shift;
                Lib::u8 blue_mask_size;
                Lib::u8 blue_mask_shift;
        };
} // namespace Kiwi::Boot