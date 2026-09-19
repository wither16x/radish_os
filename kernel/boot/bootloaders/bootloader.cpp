#include <boot/bootloaders/bootloader.hpp>

namespace Kiwi::Boot::Bootloaders
{
        const Lib::String<Bootloader::NAME_SIZE> &Bootloader::getName(this const Bootloader &self)
        {
                return self.name;
        }

        const Lib::String<Bootloader::VERSION_SIZE> &Bootloader::getVersion(this const Bootloader &self)
        {
                return self.version;
        }
} // namespace Kiwi::Boot::Bootloaders