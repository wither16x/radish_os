#pragma once

#include <fs/vfs.hpp>

namespace Kiwi::Fs::Tmpfs
{
        class Tmpfs : public Vfs::FileSystem
        {
                ~Tmpfs()           = default;

                Vfs::VNode *getRoot() override;
                Vfs::Status unmount() override;
        };
} // namespace Kiwi::Fs::Tmpfs