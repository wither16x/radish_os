#pragma once

#include <fs/vfs.hpp>
#include <lib/string.hpp>

namespace Kiwi::Fs::Devfs
{
        struct Devfs : public Vfs::FileSystem
        {
                ~Devfs()        = default;

                Vfs::VNode *getRoot() override;
                Vfs::Status unmount() override;
        };

        /// Every device type is registered here to ensure that no unsupported device can be
        /// registered.
        enum class DeviceType
        {
                None,
                Console,
                Input
        };

        void registerDevice(DeviceType type, const Lib::String &path);
} // namespace Kiwi::Fs::Devfs