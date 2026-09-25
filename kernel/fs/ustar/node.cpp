#include <fs/vfs/status.hpp>
#include <fs/ustar/node.hpp>

namespace Kiwi::Fs::Ustar
{
        Vfs::Status Node::readdir(Vfs::DirEntry *entry, Lib::usize n)
        {
                if (not this->dir_data)
                        return Vfs::Status::NotADirectory;
                if (n >= this->dir_data->nodes.length())
                        return Vfs::Status::OutOfBounds;

                Node *nd        = this->dir_data->nodes[n];
                entry->name     = nd->name;
                entry->type     = nd->dir_data ? Vfs::DirEntryType::Dir : Vfs::DirEntryType::File;

                return Vfs::Status::Success;
        }

        Vfs::Node *Node::lookup(const Lib::String<> &name)
        {
                if (not this->dir_data)
                        return nullptr;

                // because root has no parent, the loop below would not work
                if (name == "/")
                        return root;

                for (Lib::usize i = 0; i < this->dir_data->nodes.length(); i++) {
                        if (this->dir_data->nodes[i]->name == name)
                                return this->dir_data->nodes[i];
                }

                return nullptr;
        }

        Vfs::Status Node::getfilesz(Lib::usize *buf)
        {
                if (this->dir_data)
                        return Vfs::Status::IsADirectory;
                if (not this->storage)
                        return Vfs::Status::NullData;

                Lib::memcpy(buf, &this->storage->size, sizeof(*buf));
                
                return Vfs::Status::Success;
        }

        Vfs::Status Node::getdirentn(Lib::usize *buf)
        {
                if (not this->dir_data)
                        return Vfs::Status::NotADirectory;

                Lib::usize count = this->dir_data->nodes.length();
                Lib::memcpy(buf, &count, sizeof(*buf));

                return Vfs::Status::Success;
        }

        Vfs::File *Node::open()
        {
                if (this->dir_data)
                        return nullptr; // is a directory

                File *file = new File;
                file->owner = this;

                return file;
        }
} // namespace Kiwi::Fs::Ustar