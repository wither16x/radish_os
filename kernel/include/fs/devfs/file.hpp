#pragma once

#include <fs/vfs/file.hpp>
#include <fs/devfs/node.hpp>

namespace Kiwi::Fs::Devfs
{
        struct File : public Vfs::File
        {
                Node *owner;

                Vfs::Status read(void *buf, Lib::usize size) override;
                Vfs::Status write(const void *buf, Lib::usize size) override;
                Vfs::Status close() override;
        };
} // namespace Kiwi::Fs::Devfs