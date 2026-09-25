#include <fs/vfs/status.hpp>
#include <fs/devfs/node.hpp>
#include <fs/devfs/file.hpp>

namespace Kiwi::Fs::Devfs
{
        Vfs::Status Node::readdir(Vfs::DirEntry *entry, Lib::usize n)
        {
                if (this->type != NodeType::Root)
                        return Vfs::Status::IsADirectory;
                if (n >= devices.length())
                        return Vfs::Status::OutOfBounds;

                entry->name = devices[n]->name;
                entry->type = Vfs::DirEntryType::File;

                return Vfs::Status::Success;
        }

        Vfs::Status Node::getdirentn(Lib::usize *buf)
        {
                if (this->type != NodeType::Root)
                        return Vfs::Status::NotADirectory;

                Lib::usize count = devices.length();
                Lib::memcpy(buf, &count, sizeof(*buf));

                return Vfs::Status::Success;
        }

        Vfs::Node *Node::lookup(const Lib::String<> &name)
        {
                if (this->type != NodeType::Root)
                        return nullptr; // cannot lookup from a device

                if (name == "/")
                        return root;

                for (Lib::usize i = 0; i < devices.length(); i++) {
                        if (devices[i]->name == name)
                                return devices[i];
                }

                return nullptr; // node not found
        }

        Vfs::File *Node::open()
        {
                if (this->type != NodeType::Device)
                        return nullptr; // can only open device files

                if (not this->file_data) {
                        this->file_data = new File;
                        this->file_data->owner = this;
                }

                return this->file_data;
        }
} // namespace Kiwi::Fs::Devfs