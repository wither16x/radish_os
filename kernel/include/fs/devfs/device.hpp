#pragma once

#include <lib/string.hpp>
#include <fs/devfs/decls.hpp>

namespace Kiwi::Fs::Devfs
{
        /// Every device type is registered here to ensure that no unsupported device can be
        /// registered.
        enum class DeviceType
        {
                None,
                Console,
                Input
        };

        void registerDevice(DeviceType type, const Lib::String<> &path);

        inline Lib::Vector<Node *> devices;
} // namespace Kiwi::Fs::Devfs