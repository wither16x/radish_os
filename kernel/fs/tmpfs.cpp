#include <fs/path.hpp>
#include <fs/tmpfs.hpp>
#include <fs/vfs.hpp>
#include <lib/memory.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>

// I don't test tmpfs a lot so there might be bugs here...

namespace Kiwi::Fs::Tmpfs
{
        namespace
        {
                /// Enumeration of the TMPFS node types.
                enum class NodeType
                {
                        File,
                        Dir
                };

                class Storage
                {
                public:
                        char *data = nullptr;
                        Lib::usize size = 0;
                };

                class TmpfsDir
                {
                public:
                        Lib::Vector<class Node *> nodes;
                };

                class TmpfsFile : public Vfs::File {
                public:
                        Node *owner;

                        Vfs::Status read(void *buf, Lib::usize size) override;
                        Vfs::Status write(const void *buf, Lib::usize size) override;
                        Vfs::Status close() override;
                };

                // Depending on the node type, `file_data` or `dir_data` is set
                // to `nullptr`
                class Node : public Vfs::VNode
                {
                public:
                        Node *parent;
                        NodeType type;
                        Lib::String name;
                        Storage *storage;
                        TmpfsFile *file_data;
                        TmpfsDir *dir_data;

                        Vfs::Status mkfile(const Lib::String &name) override;
                        Vfs::Status mkdir(const Lib::String &name) override;
                        Vfs::Status rm() override;
                        Vfs::Status readdir(Vfs::DirEntry *entry, Lib::usize index) override;
                        Vfs::VNode *lookup(const Lib::String &name) override;
                        Vfs::Status getfilesz(Lib::usize *buf) override;
                        Vfs::Status getdirentn(Lib::usize *buf) override;
                        Vfs::File *open() override;
                };

                Node *root = nullptr;

                /// Recursive on `NodeType::Dir`.
                void removeNode(Node *node)
                {
                        if (not node)
                                return;      // node is null

                        if (node->dir_data) {
                                for (Lib::usize i = 0; i < node->dir_data->nodes.size(); i++)
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
        } // anonymous namespace

        Vfs::Status TmpfsFile::read(void *buf, Lib::usize n)
        {
                if (not this->owner or not this->owner->storage)
                        return Vfs::Status::NullData;
                if (this->owner->storage->size >= n)
                        return Vfs::Status::OutOfBounds;

                Lib::memcpy(buf, this->owner->storage->data, this->owner->storage->size);

                return Vfs::Status::Success;
        }

        Vfs::Status TmpfsFile::write(const void *buf, Lib::usize n)
        {
                if (not this->owner or not this->owner->storage)
                        return Vfs::Status::NullData;

                Storage *storage = this->owner->storage;
                if (storage->data)
                        delete[] storage->data;

                storage->data = new char[n + 1];
                storage->size = n;

                Lib::memcpy(storage->data, buf, n);

                return Vfs::Status::Success;
        }

        Vfs::Status TmpfsFile::close()
        {
                if (this->owner)
                        this->owner->file_data = nullptr;

                return Vfs::Status::Success;
        }

        Vfs::Status Node::mkfile(const Lib::String &name)
        {
                if (this->type != NodeType::Dir)
                        return Vfs::Status::NotADirectory;

                Node *nd                = new Node;
                nd->parent              = this;
                nd->type                = NodeType::File;
                nd->name                = name;
                nd->file_data           = new TmpfsFile;
                nd->file_data           = nullptr;
                nd->dir_data            = nullptr;
                nd->storage             = new Storage;
                nd->ref_count           = 1;

                this->dir_data->nodes.pushBack(nd);

                return Vfs::Status::Success;
        }

        Vfs::Status Node::mkdir(const Lib::String &name)
        {
                if (this->type != NodeType::Dir)
                        return Vfs::Status::NotADirectory;

                Node *nd                = new Node;
                nd->parent              = this;
                nd->type                = NodeType::Dir;
                nd->name                = name;
                nd->file_data           = nullptr;
                nd->storage             = nullptr;
                nd->dir_data            = new TmpfsDir;
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

                        for (Lib::usize i = 0; i < siblings.size(); i++) {
                                if (siblings[i] == this) {
                                        siblings.erase(i);
                                        break;
                                }
                        }
                }

                if (this->dir_data) {
                        for (Lib::usize i = 0; i < this->dir_data->nodes.size(); i++)
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
                if (n >= this->dir_data->nodes.size())
                        return Vfs::Status::OutOfBounds;

                Node *nd        = this->dir_data->nodes[n];
                entry->name     = nd->name;
                entry->type     = nd->type == NodeType::Dir ? Vfs::DirEntryType::Dir
                                        : Vfs::DirEntryType::File
                                ;

                return Vfs::Status::Success;
        }

        Vfs::VNode *Node::lookup(const Lib::String &name)
        {
                if (this->type != NodeType::Dir)
                        return nullptr;

                if (name == "/")
                        return root;

                for (Lib::usize i = 0; i < this->dir_data->nodes.size(); i++) {
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
                
                Lib::usize count = this->dir_data->nodes.size();
                Lib::memcpy(buf, &count, sizeof(*buf));

                return Vfs::Status::Success;;
        }

        Vfs::File *Node::open()
        {
                if (this->type != NodeType::File)
                        return nullptr;

                if (not this->file_data) {
                        this->file_data = new TmpfsFile;
                        this->file_data->owner = this;
                }

                return this->file_data;
        }

        Vfs::VNode *Tmpfs::getRoot()
        {
                if (not root) {
                        root            = new Node;
                        root->parent    = nullptr;
                        root->name      = "/";
                        root->type      = NodeType::Dir;
                        root->dir_data  = new TmpfsDir;
                        root->storage   = nullptr;
                        root->file_data = nullptr;
                        root->ref_count = 1;
                }

                return root;
        }

        Vfs::Status Tmpfs::unmount()
        {
                if (root) {
                        if (root->dir_data) {
                                for (Lib::usize i = 0; i < root->dir_data->nodes.size(); i++)
                                        removeNode(root->dir_data->nodes[i]);
                                delete root->dir_data;
                        }

                        delete root;
                        root = nullptr;
                }

                return Vfs::Status::Success;
        }
} // namespace Kiwi::Fs::Tmpfs