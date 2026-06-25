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

// Depending on the node type, `file_data` or `dir_data` is set
// to `nullptr`
struct Node : public vfs::VNode{
        Node *parent;

        NodeType type;
        String name;

        struct File *file_data;
        struct Dir *dir_data;

        int create_file(const String &name) override;
        int create_dir(const String &name) override;
        int remove() override;
        int write_file(const char *buf, usize n) override;
        int read_file(char *buf, usize n) override;
        int readdir(vfs::DirEntry *entry, usize n) override;
        void *lookup(const String &name) override;
        int getfilesz(usize *buf) override;
        int getdirentn(usize *buf) override;
};

struct File {
        char *data;
        usize size;
};

struct Dir {
        Vector<Node *> nodes;
};

Node *root = nullptr;

/// Recursive on `NodeType::Dir`.
int remove_node(Node *node)
{
        if (!node)
                return -1;      // node is null

        if (node->dir_data) {
                for (usize i = 0; i < node->dir_data->nodes.size(); i++)
                        remove_node(node->dir_data->nodes[i]);

                delete node->dir_data;
        }

        if (node->file_data) {
                if (node->file_data->data)
                        delete[] node->file_data->data;

                delete node->file_data;
        }

        delete node;

        return 0;
}

} /* anonymous namespace */

// --------------------------------------------------
int Node::create_file(const String &name)
{
        if (this->type != NodeType::Dir)
                return -1;              // files can only be created in directories

        Node *nd                = new Node;
        nd->parent              = this;
        nd->type                = NodeType::File;
        nd->name                = name;
        nd->file_data           = new File;
        nd->file_data->data     = nullptr;
        nd->file_data->size     = 0;
        nd->dir_data            = nullptr;

        this->dir_data->nodes.push_back(nd);

        return 0;
}
// --------------------------------------------------

// --------------------------------------------------
int Node::create_dir(const String &name)
{
        if (this->type != NodeType::Dir)
                return -1;      // directories can only be created in directories

        Node *nd                = new Node;
        nd->parent              = this;
        nd->type                = NodeType::Dir;
        nd->name                = name;
        nd->file_data           = nullptr;
        nd->dir_data            = new Dir;

        this->dir_data->nodes.push_back(nd);

        return 0;
}
// --------------------------------------------------

// --------------------------------------------------
int Node::remove()
{
        // detach the node before removing it so that the parent directory
        // does not contain a pointer to the freed memory
        if (this->parent && this->parent->dir_data) {
                Vector<Node *> &siblings = this->parent->dir_data->nodes;

                for (usize i = 0; i < siblings.size(); i++) {
                        if (siblings[i] == this) {
                                siblings.erase(i);
                                break;
                        }
                }
        }

        return remove_node(this);
}
// --------------------------------------------------

// --------------------------------------------------
int Node::write_file(const char *buf, usize n)
{
        if (this->type != NodeType::File)
                return -1;              // cannot write in directories like that

        if (this->file_data->data)
                delete[] this->file_data->data;

        this->file_data->data = new char[n + 1];
        this->file_data->size = n;

        memcpy(this->file_data->data, buf, n);

        return 0;
}
// --------------------------------------------------

// --------------------------------------------------
int Node::read_file(char *buf, usize n)
{
        if (this->type != NodeType::File)
                return -1;              // cannot read directories like that
        if (!this->file_data || !this->file_data->data)
                return -2;              // file has no valid data
        if (this->file_data->size >= n)
                return -3;              // buffer too small

        memcpy(buf, this->file_data->data, this->file_data->size);

        return 0;
}
// --------------------------------------------------

// --------------------------------------------------
int Node::readdir(vfs::DirEntry *entry, usize n)
{
        if (this->type != NodeType::Dir)
                return -1;              // not a directory

        Node *nd = this->dir_data->nodes[n];
        entry->name = nd->name;
        entry->is_dir = nd->type == NodeType::Dir;

        return 0;
}
// --------------------------------------------------

// --------------------------------------------------
void *Node::lookup(const String &name)
{
        if (this->type != NodeType::Dir)
                return nullptr;

        for (usize i = 0; i < this->dir_data->nodes.size(); i++) {
                if (this->dir_data->nodes[i]->name == name)
                        return this->dir_data->nodes[i];
        }

        return nullptr;
}
// --------------------------------------------------

// --------------------------------------------------
int Node::getfilesz(usize *buf)
{
        if (this->type != NodeType::File)
                return -1;      // not a file

        memcpy(buf, &this->file_data->size, sizeof(*buf));

        return 0;
}
// --------------------------------------------------

// --------------------------------------------------
int Node::getdirentn(usize *buf)
{
        if (this->type != NodeType::Dir)
                return -1;      // not a directory
        
        usize count = this->dir_data->nodes.size();
        memcpy(buf, &count, sizeof(*buf));

        return 0;
}
// --------------------------------------------------

// --------------------------------------------------
vfs::VNode *TMPFS::get_root()
{
        if (!root) {
                root            = new Node;
                root->parent    = nullptr;
                root->name      = "/";
                root->type      = NodeType::Dir;
                root->dir_data  = new Dir;
                root->file_data = nullptr;
                root->owned     = false;
        }

        return root;
}
// --------------------------------------------------

// --------------------------------------------------
void TMPFS::unmount()
{
        root->remove();
        root = nullptr;
}
// --------------------------------------------------

} /* namespace kernel::fs::tmpfs */