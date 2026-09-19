#pragma once

#include <lib/typing.hpp>
#include <boot/bootloaders/bootloader.hpp>

namespace Kiwi::Boot::Bootloaders
{
        namespace LimineSpecific
        {
                struct Revision : BootloaderVersionInfo
                {
                        Lib::u64 magic0;
                        Lib::u64 magic1;
                        Lib::u64 rev;
                };

                struct RequestsStartMarker : BootloaderRequestsStart
                {
                        Lib::u64 magic0;
                        Lib::u64 magic1;
                        Lib::u64 magic2;
                        Lib::u64 magic3;
                };

                struct RequestsEndMarker : BootloaderRequestsEnd
                {
                        Lib::u64 magic0;
                        Lib::u64 magic1;
                };
        } // namespace LimineSpecific

        class Limine : public Bootloader
        {
                LimineSpecific::Revision base_revision;
                LimineSpecific::RequestsStartMarker requests_start;
                LimineSpecific::RequestsEndMarker requests_end;

        public:
                void init(const Lib::String<NAME_SIZE> &name, const Lib::String<VERSION_SIZE> &version) override;

                const volatile LimineSpecific::RequestsStartMarker &requestsStart() override;
                const volatile LimineSpecific::RequestsEndMarker &requestsEnd() override;
                const volatile LimineSpecific::Revision &getVersionInfo() override;

                bool isBaseRevisionSupported() const;
        };
} // namespace Kiwi::Boot::Bootloaders