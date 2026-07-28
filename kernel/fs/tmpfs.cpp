#include <fs/path.hpp>
#include <fs/tmpfs.hpp>
#include <fs/vfs.hpp>
#include <lib/memory.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>

// I don't test tmpfs a lot so there might be bugs here...

using kernel::lib::String;
using kernel::lib::usize;
using kernel::lib::memcpy;
using kernel::lib::Vector;

namespace kernel::fs::tmpfs {

namespace {

/// Enumeration of the TMPFS node types.
enum class NodeType : int {
        File,
        Dir
};

class Storage {
public:
        char *data = nullptr;
        usize size = 0;
};

class TmpfsDir {
public:
        Vector<class Node *> nodes;
};

class TmpfsFile : public vfs::File {
public:
        Node *owner;

        vfs::Status read(void *buf, usize size) override;
        vfs::Status write(const void *buf, usize size) override;
        vfs::Status close() override;
};

// Depending on the node type, `file_data` or `dir_data` is set
// to `nullptr`
class Node : public vfs::VNode {
public:
        Node *parent;

        NodeType type;
        String name;

        Storage *storage;
        TmpfsFile *file_data;
        TmpfsDir *dir_data;

        vfs::Status mkfile(const String &name) override;
        vfs::Status mkdir(const String &name) override;
        vfs::Status rm() override;
        vfs::Status readdir(vfs::DirEntry *entry, usize index) override;
        vfs::VNode *lookup(const String &name) override;
        vfs::Status getfilesz(usize *buf) override;
        vfs::Status getdirentn(usize *buf) override;
        vfs::File *open() override;
};

Node *root = nullptr;

/// Recursive on `NodeType::Dir`.
void remove_node(Node *node)
{
        if (!node)
                return;      // node is null

        if (node->dir_data) {
                for (usize i = 0; i < node->dir_data->nodes.size(); i++)
                        remove_node(node->dir_data->nodes[i]);

                delete node->dir_data;
        }

        if (node->storage) {
                if (node->storage->data)
                        delete[] node->storage->data;
                delete node->storage;
        }

        delete node;
}

} /* anonymous namespace */

// --------------------------------------------------
vfs::Status TmpfsFile::read(void *buf, usize n)
{
        if (not this->owner or not this->owner->storage)
                return vfs::Status::NullData;
        if (this->owner->storage->size >= n)
                return vfs::Status::OutOfBounds;

        memcpy(buf, this->owner->storage->data, this->owner->storage->size);

        return vfs::Status::Success;
}
// --------------------------------------------------

// --------------------------------------------------
vfs::Status TmpfsFile::write(const void *buf, usize n)
{
        if (not this->owner or not this->owner->storage)
                return vfs::Status::NullData;

        Storage *storage = this->owner->storage;
        if (storage->data)
                delete[] storage->data;

        storage->data = new char[n + 1];
        storage->size = n;

        memcpy(storage->data, buf, n);

        return vfs::Status::Success;
}
// --------------------------------------------------

vfs::Status TmpfsFile::close()
{
        if (this->owner)
                this->owner->file_data = nullptr;

        return vfs::Status::Success;
}

// --------------------------------------------------
vfs::Status Node::mkfile(const String &name)
{
        if (this->type != NodeType::Dir)
                return vfs::Status::NotADirectory;

        Node *nd                = new Node;
        nd->parent              = this;
        nd->type                = NodeType::File;
        nd->name                = name;
        nd->file_data           = new TmpfsFile;
        nd->file_data           = nullptr;
        nd->dir_data            = nullptr;
        nd->storage             = new Storage;
        nd->ref_count           = 1;

        this->dir_data->nodes.push_back(nd);

        return vfs::Status::Success;
}
// --------------------------------------------------

// --------------------------------------------------
vfs::Status Node::mkdir(const String &name)
{
        if (this->type != NodeType::Dir)
                return vfs::Status::NotADirectory;

        Node *nd                = new Node;
        nd->parent              = this;
        nd->type                = NodeType::Dir;
        nd->name                = name;
        nd->file_data           = nullptr;
        nd->storage             = nullptr;
        nd->dir_data            = new TmpfsDir;
        nd->ref_count           = 1;

        this->dir_data->nodes.push_back(nd);

        return vfs::Status::Success;
}
// --------------------------------------------------

// --------------------------------------------------
vfs::Status Node::rm()
{
        // detach the node before removing it so that the parent directory
        // does not contain a pointer to the freed memory
        if (this->parent and this->parent->dir_data) {
                Vector<Node *> &siblings = this->parent->dir_data->nodes;

                for (usize i = 0; i < siblings.size(); i++) {
                        if (siblings[i] == this) {
                                siblings.erase(i);
                                break;
                        }
                }
        }

        if (this->dir_data) {
                for (usize i = 0; i < this->dir_data->nodes.size(); i++)
                        remove_node(this->dir_data->nodes[i]);

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

        return vfs::release_node(this);
}
// --------------------------------------------------

// --------------------------------------------------
vfs::Status Node::readdir(vfs::DirEntry *entry, usize n)
{
        if (this->type != NodeType::Dir)
                return vfs::Status::NotADirectory;
        if (n >= this->dir_data->nodes.size())
                return vfs::Status::OutOfBounds;

        Node *nd        = this->dir_data->nodes[n];
        entry->name     = nd->name;
        entry->type     = nd->type == NodeType::Dir ? vfs::DirEntryType::Dir
                                : vfs::DirEntryType::File
                        ;

        return vfs::Status::Success;
}
// --------------------------------------------------

// --------------------------------------------------
vfs::VNode *Node::lookup(const String &name)
{
        if (this->type != NodeType::Dir)
                return nullptr;

        if (name == "/")
                return root;

        for (usize i = 0; i < this->dir_data->nodes.size(); i++) {
                if (this->dir_data->nodes[i]->name == name)
                        return this->dir_data->nodes[i];
        }

        return nullptr;
}
// --------------------------------------------------

// --------------------------------------------------
vfs::Status Node::getfilesz(usize *buf)
{
        if (this->type != NodeType::File)
                return vfs::Status::IsADirectory;

        memcpy(buf, &this->file_data->size, sizeof(*buf));

        return vfs::Status::Success;
}
// --------------------------------------------------

// --------------------------------------------------
vfs::Status Node::getdirentn(usize *buf)
{
        if (this->type != NodeType::Dir)
                return vfs::Status::NotADirectory;
        
        usize count = this->dir_data->nodes.size();
        memcpy(buf, &count, sizeof(*buf));

        return vfs::Status::Success;;
}
// --------------------------------------------------

vfs::File *Node::open()
{
        if (this->type != NodeType::File)
                return nullptr;

        if (not this->file_data) {
                this->file_data = new TmpfsFile;
                this->file_data->owner = this;
        }

        return this->file_data;
}

// --------------------------------------------------
vfs::VNode *TMPFS::get_root()
{
        if (!root) {
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
// --------------------------------------------------

// --------------------------------------------------
vfs::Status TMPFS::unmount()
{
        if (root) {
                if (root->dir_data) {
                        for (usize i = 0; i < root->dir_data->nodes.size(); i++)
                                remove_node(root->dir_data->nodes[i]);
                        delete root->dir_data;
                }

                delete root;
                root = nullptr;
        }

        return vfs::Status::Success;
}
// --------------------------------------------------

} /* namespace kernel::fs::tmpfs */