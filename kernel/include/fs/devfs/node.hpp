#pragma once

#include <fs/vfs/node.hpp>
#include <fs/devfs/device.hpp>
#include <fs/devfs/decls.hpp>

namespace Kiwi::Fs::Devfs
{
        enum class NodeType
        {
                Root,
                Device
        };

        struct Node : public Vfs::Node
        {
                Lib::String<> name;
                NodeType type;
                DeviceType devtype;
                File *file_data;

                Vfs::Status mkfile(const Lib::String<> &name) override;
                Vfs::Status rm() override;
                Vfs::Status readdir(Vfs::DirEntry *entry, Lib::usize index) override;
                Vfs::Node *lookup(const Lib::String<> &name) override;
                Vfs::Status getdirentn(Lib::usize *buf) override;
                Vfs::File *open() override;
        };

        inline Node *root = nullptr;
} // namespace Kiwi::Fs::Devfs