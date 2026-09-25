#pragma once

#include <fs/vfs/node.hpp>
#include <fs/ustar/file.hpp>
#include <fs/ustar/dir.hpp>

namespace Kiwi::Fs::Ustar
{
        enum class NodeType : char
        {
                NormalFile      = '0',
                HardLink        = '1',
                SymbolicLink    = '2',
                CharSpecialDev  = '3',
                BlockDev        = '4',
                Directory       = '5',
                NamedPipe       = '6'
        };

        struct Node : Vfs::Node
        {
                Node *parent;
                FileHeader *hdr;
                Lib::String<> name;
                Storage *storage;
                Dir *dir_data;

                Vfs::Status readdir(Vfs::DirEntry *entry, Lib::usize index) override;
                Vfs::Node *lookup(const Lib::String<> &name) override;
                Vfs::Status getfilesz(Lib::usize *buf) override;
                Vfs::Status getdirentn(Lib::usize *buf) override;
                Vfs::File *open() override;
        };

        inline Node *root = nullptr;
} // namespace Kiwi::Fs::Ustar