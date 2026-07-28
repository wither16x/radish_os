#pragma once

#include <fs/vfs.hpp>
#include <lib/string.hpp>

namespace kernel::fs::devfs {

struct DEVFS : public vfs::FileSystem {
        ~DEVFS()        = default;

        vfs::VNode *get_root() override;
        vfs::Status unmount() override;
};

/// Every device type is registered here to ensure that no unsupported device can be
/// registered.
enum class DeviceType : int {
        None,
        Console,
        Input
};

void register_device(DeviceType type, const lib::String &path);

} /* namespace kernel::fs::devfs */