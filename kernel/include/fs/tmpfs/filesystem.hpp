#pragma once

#include <fs/vfs/filesystem.hpp>
#include <fs/vfs/node.hpp>

namespace Kiwi::Fs::Tmpfs
{
        class Tmpfs : public Vfs::FileSystem
        {
                ~Tmpfs()           = default;

                Vfs::Node *getRoot() override;
                Lib::Result<bool, Vfs::Status> unmount() override;
        };
} // namespace Kiwi::Fs::Tmpfs