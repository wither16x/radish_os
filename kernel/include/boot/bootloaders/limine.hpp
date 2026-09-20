#pragma once

#include <lib/typing.hpp>
#include <boot/requests.hpp>

namespace Kiwi::Boot::Bootloaders
{
        namespace LimineSpecific
        {
                struct Revision
                {
                        Lib::u64 magic0;
                        Lib::u64 magic1;
                        Lib::u64 rev;
                };

                struct RequestsStartMarker
                {
                        Lib::u64 magic0;
                        Lib::u64 magic1;
                        Lib::u64 magic2;
                        Lib::u64 magic3;
                };

                struct RequestsEndMarker
                {
                        Lib::u64 magic0;
                        Lib::u64 magic1;
                };
        } // namespace LimineSpecific

        class Limine
        {
                LimineSpecific::Revision base_revision;
                LimineSpecific::RequestsStartMarker requests_start;
                LimineSpecific::RequestsEndMarker requests_end;

                BootloaderInfoRequest req_bootloader_info;
                FirmwareTypeRequest req_firmware_type;
                MemmapRequest req_memmap;
                HhdmRequest req_hhdm;
                ExecutableAddressRequest req_executable_address;
                ModuleRequest req_module;
                FramebufferRequest req_framebuffer;

        public:
                void init(this Limine &self);

                const Request &request(this const Limine &self, RequestType type);

                const LimineSpecific::RequestsStartMarker &requestsStart(this const Limine &self);
                const LimineSpecific::RequestsEndMarker &requestsEnd(this const Limine &self);
                const LimineSpecific::Revision &getVersionInfo(this const Limine &self);

                bool isBaseRevisionSupported(this const Limine &self);
        };
} // namespace Kiwi::Boot::Bootloaders