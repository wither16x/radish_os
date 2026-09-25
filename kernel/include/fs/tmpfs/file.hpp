#pragma once

#include <fs/vfs/file.hpp>
#include <fs/tmpfs/decls.hpp>

namespace Kiwi::Fs::Tmpfs
{
        struct Storage
        {
                char *data = nullptr;
                Lib::usize size = 0;
        };

        struct File : Vfs::File
        {
                Node *owner;

                Vfs::Status read(void *buf, Lib::usize size) override;
                Vfs::Status write(const void *buf, Lib::usize size) override;
                Vfs::Status close() override;
        };
} // namespace Kiwi::Fs::Tmpfs