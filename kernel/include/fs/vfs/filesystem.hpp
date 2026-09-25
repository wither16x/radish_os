#pragma once

#include <lib/result.hpp>
#include <fs/vfs/status.hpp>
#include <fs/vfs/node.hpp>

namespace Kiwi::Fs::Vfs
{
        struct FileSystem
        {
                virtual ~FileSystem() = default;

                virtual Node *getRoot() = 0;
                virtual Lib::Result<bool, Status> unmount() = 0;
        };
} // namespace Kiwi::Fs::Vfs