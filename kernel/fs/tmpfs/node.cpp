#include <fs/tmpfs/node.hpp>

namespace Kiwi::Fs::Tmpfs
{
        /// Recursive on `NodeType::Dir`.
        void removeNode(Node *node)
        {
                if (not node)
                        return;      // node is null

                if (node->dir_data) {
                        for (Lib::usize i = 0; i < node->dir_data->nodes.length(); i++)
                                removeNode(node->dir_data->nodes[i]);
                        delete node->dir_data;
                }

                if (node->storage) {
                        if (node->storage->data)
                                delete[] node->storage->data;
                        delete node->storage;
                }

                delete node;
        }

        Vfs::Status Node::mkfile(const Lib::String<> &name)
        {
                if (this->type != NodeType::Dir)
                        return Vfs::Status::NotADirectory;

                Node *nd                = new Node;
                nd->parent              = this;
                nd->type                = NodeType::File;
                nd->name                = name;
                nd->file_data           = new File;
                nd->file_data           = nullptr;
                nd->dir_data            = nullptr;
                nd->storage             = new Storage;
                nd->ref_count           = 1;

                this->dir_data->nodes.pushBack(nd);

                return Vfs::Status::Success;
        }

        Vfs::Status Node::mkdir(const Lib::String<> &name)
        {
                if (this->type != NodeType::Dir)
                        return Vfs::Status::NotADirectory;

                Node *nd                = new Node;
                nd->parent              = this;
                nd->type                = NodeType::Dir;
                nd->name                = name;
                nd->file_data           = nullptr;
                nd->storage             = nullptr;
                nd->dir_data            = new Dir;
                nd->ref_count           = 1;

                this->dir_data->nodes.pushBack(nd);

                return Vfs::Status::Success;
        }

        Vfs::Status Node::rm()
        {
                // detach the node before removing it so that the parent directory
                // does not contain a pointer to the freed memory
                if (this->parent and this->parent->dir_data) {
                        Lib::Vector<Node *> &siblings = this->parent->dir_data->nodes;

                        for (Lib::usize i = 0; i < siblings.length(); i++) {
                                if (siblings[i] == this) {
                                        siblings.erase(i);
                                        break;
                                }
                        }
                }

                if (this->dir_data) {
                        for (Lib::usize i = 0; i < this->dir_data->nodes.length(); i++)
                                removeNode(this->dir_data->nodes[i]);

                        delete this->dir_data;
                        this->dir_data = nullptr;
                }

                if (this->storage) {
                        if (this->storage->data)
                                delete[] this->storage->data;
                        delete this->storage;
                        this->storage = nullptr;
                }

                if (this->file_data) {
                        delete this->file_data;
                        this->file_data = nullptr;
                }

                return Vfs::releaseNode(this);
        }

        Vfs::Status Node::readdir(Vfs::DirEntry *entry, Lib::usize n)
        {
                if (this->type != NodeType::Dir)
                        return Vfs::Status::NotADirectory;
                if (n >= this->dir_data->nodes.length())
                        return Vfs::Status::OutOfBounds;

                Node *nd        = this->dir_data->nodes[n];
                entry->name     = nd->name;
                entry->type     = nd->type == NodeType::Dir ? Vfs::DirEntryType::Dir
                                        : Vfs::DirEntryType::File
                                ;

                return Vfs::Status::Success;
        }

        Vfs::Node *Node::lookup(const Lib::String<> &name)
        {
                if (this->type != NodeType::Dir)
                        return nullptr;

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
                if (this->type != NodeType::File)
                        return Vfs::Status::IsADirectory;

                Lib::memcpy(buf, &this->file_data->size, sizeof(*buf));

                return Vfs::Status::Success;
        }

        Vfs::Status Node::getdirentn(Lib::usize *buf)
        {
                if (this->type != NodeType::Dir)
                        return Vfs::Status::NotADirectory;
                
                Lib::usize count = this->dir_data->nodes.length();
                Lib::memcpy(buf, &count, sizeof(*buf));

                return Vfs::Status::Success;;
        }

        Vfs::File *Node::open()
        {
                if (this->type != NodeType::File)
                        return nullptr;

                if (not this->file_data) {
                        this->file_data = new File;
                        this->file_data->owner = this;
                }

                return this->file_data;
        }
} // namespace Kiwi::Fs::Tmpfs