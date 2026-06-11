#include <fs/path.hpp>
#include <fs/tmpfs.hpp>
#include <fs/vfs.hpp>
#include <lib/memory.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>

using kernel::lib::String;
using kernel::lib::usize;
using kernel::lib::memcpy;
using kernel::lib::Vector;

namespace kernel::fs::tmpfs {

namespace {

enum class NodeType : int {
        File,
        Dir
};

// Depending on the node type, `file_data` or `dir_data` is set
// to `nullptr`
struct Node {
        Node *parent;

        NodeType type;
        String name;

        struct File *file_data;
        struct Dir *dir_data;
};

struct File {
        char *data;
        usize size;
};

struct Dir {
        Vector<Node *> nodes;
};

Node *root = nullptr;

// recursive on `NodeType::Dir`
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

int create_file(void *fs_data, const String &name)
{
        Node *dir = static_cast<Node *>(fs_data);
        if (dir->type != NodeType::Dir)
                return -1;              // files can only be created in directories

        Node *nd                = new Node;
        nd->parent              = dir;
        nd->type                = NodeType::File;
        nd->name                = name;
        nd->file_data           = new File;
        nd->file_data->data     = nullptr;
        nd->file_data->size     = 0;
        nd->dir_data            = nullptr;

        dir->dir_data->nodes.push_back(nd);

        return 0;
}

int create_dir(void *fs_data, const String &name)
{
        Node *dir = static_cast<Node *>(fs_data);
        if (dir->type != NodeType::Dir)
                return -1;      // directories can only be created in directories

        Node *nd                = new Node;
        nd->parent              = dir;
        nd->type                = NodeType::Dir;
        nd->name                = name;
        nd->file_data           = nullptr;
        nd->dir_data            = new Dir;

        dir->dir_data->nodes.push_back(nd);

        return 0;
}

int remove(void *fs_data)
{
        Node *nd = static_cast<Node *>(fs_data);
        if (!nd)
                return -1;              // node is null

        // detach the node before removing it so that the parent directory
        // does not contain a pointer to the freed memory
        if (nd->parent && nd->parent->dir_data) {
                Vector<Node *> &siblings = nd->parent->dir_data->nodes;

                for (usize i = 0; i < siblings.size(); i++) {
                        if (siblings[i] == nd) {
                                siblings.erase(i);
                                break;
                        }
                }
        }

        return remove_node(nd);
}

int write_file(void *fs_data, const char *buf, usize n)
{
        Node *f = static_cast<Node *>(fs_data);
        if (f->type != NodeType::File)
                return -1;              // cannot write in directories like that

        if (f->file_data->data)
                delete[] f->file_data->data;

        f->file_data->data = new char[n + 1];
        f->file_data->size = n;

        memcpy(f->file_data->data, buf, n);

        return 0;
}

int read_file(void *fs_data, char *buf, usize n)
{
        Node *f = static_cast<Node *>(fs_data);
        if (f->type != NodeType::File)
                return -1;              // cannot read directories like that
        if (!f->file_data || !f->file_data->data)
                return -2;              // file has no valid data
        if (f->file_data->size >= n)
                return -3;              // buffer too small

        memcpy(buf, f->file_data->data, f->file_data->size);

        return 0;
}

int readdir(void *fs_data, Vector<vfs::DirEntry> &entries)
{
        Node *dir = static_cast<Node *>(fs_data);
        if (dir->type != NodeType::Dir)
                return -1;              // not a directory

        for (usize i = 0; i < dir->dir_data->nodes.size(); i++) {
                Node *child = dir->dir_data->nodes[i];
                vfs::DirEntry entry;

                entry.name = child->name;
                entry.is_dir = child->type == NodeType::Dir;

                entries.push_back(entry);
        }

        return 0;
}

void *lookup(void *fs_data, const String &name)
{
        Node *dir = static_cast<Node *>(fs_data);
        if (dir->type != NodeType::Dir)
                return nullptr;

        for (usize i = 0; i < dir->dir_data->nodes.size(); i++) {
                if (dir->dir_data->nodes[i]->name == name)
                        return dir->dir_data->nodes[i];
        }

        return nullptr;
}

int get_file_size(void *fs_data, usize *buf)
{
        Node *f = static_cast<Node *>(fs_data);
        if (f->type != NodeType::File)
                return 0;      // file not found

        memcpy(buf, &f->file_data->size, sizeof(*buf));

        return 0;
}

// tmpfs operations
vfs::VNodeOps tmpfs_ops = {
        .create_file    = create_file,
        .create_dir     = create_dir,
        .remove         = remove,
        .write_file     = write_file,
        .read_file      = read_file,
        .readdir        = readdir,
        .lookup         = lookup,
        .get_file_size  = get_file_size
};

} /* anonymous namespace */

vfs::VNode *TMPFS::get_root()
{
        if (!root) {
                root            = new Node;
                root->parent    = nullptr;
                root->name      = "/";
                root->type      = NodeType::Dir;
                root->dir_data  = new Dir;
                root->file_data = nullptr;
        }

        vfs::VNode *vnd = new vfs::VNode;
        vnd->ops        = &tmpfs_ops;
        vnd->fs_data    = root;
        vnd->owned      = false;

        return vnd;
}

void TMPFS::unmount()
{
        remove_node(root);
        root = nullptr;
}

} /* namespace kernel::fs::tmpfs */