#pragma once

#include <lib/string.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Boot::Bootloaders
{
        struct BootloaderRequestsStart
        {};

        struct BootloaderRequestsEnd
        {};

        struct BootloaderVersionInfo
        {};

        class Bootloader
        {
        protected:
                static constexpr Lib::usize NAME_SIZE = 255;
                static constexpr Lib::usize VERSION_SIZE = 255;

                Lib::String<NAME_SIZE> name;
                Lib::String<VERSION_SIZE> version;

        public:
                virtual void init(const Lib::String<NAME_SIZE> &name, const Lib::String<VERSION_SIZE> &version) = 0;

                const Lib::String<NAME_SIZE> &getName(this const Bootloader &self);
                const Lib::String<VERSION_SIZE> &getVersion(this const Bootloader &self);

                virtual const volatile BootloaderRequestsStart &requestsStart() = 0;
                virtual const volatile BootloaderRequestsEnd &requestsEnd() = 0;
                virtual const volatile BootloaderVersionInfo &getVersionInfo() = 0;
        };
} // namespace Kiwi::Boot::Bootloaders