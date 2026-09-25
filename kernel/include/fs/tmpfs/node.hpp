#pragma once

#include <fs/vfs/node.hpp>
#include <fs/tmpfs/file.hpp>
#include <fs/tmpfs/dir.hpp>

namespace Kiwi::Fs::Tmpfs
{
        enum class NodeType
        {
                File,
                Dir
        };

        struct Node : Vfs::Node
        {
                Node *parent;
                NodeType type;
                Lib::String<> name;
                Storage *storage;
                File *file_data;
                Dir *dir_data;

                Vfs::Status mkfile(const Lib::String<> &name) override;
                Vfs::Status mkdir(const Lib::String<> &name) override;
                Vfs::Status rm() override;
                Vfs::Status readdir(Vfs::DirEntry *entry, Lib::usize index) override;
                Vfs::Node *lookup(const Lib::String<> &name) override;
                Vfs::Status getfilesz(Lib::usize *buf) override;
                Vfs::Status getdirentn(Lib::usize *buf) override;
                Vfs::File *open() override;
        };

        void removeNode(Node *node);

        inline Node *root = nullptr;
} // namespace Kiwi::Fs::Tmpfs