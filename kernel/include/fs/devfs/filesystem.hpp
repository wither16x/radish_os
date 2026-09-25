#pragma once

#include <fs/vfs/filesystem.hpp>
#include <fs/vfs/node.hpp>

namespace Kiwi::Fs::Devfs
{
        struct Devfs : public Vfs::FileSystem
        {
                ~Devfs()        = default;

                Vfs::Node *getRoot() override;
                Lib::Result<bool, Vfs::Status> unmount() override;
        };
} // namespace Kiwi::Fs::Devfs