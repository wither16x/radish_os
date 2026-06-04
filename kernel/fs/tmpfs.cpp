#include <fs/path.hpp>
#include <fs/tmpfs.hpp>
#include <lib/memory.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>

using kernel::lib::String;
using kernel::lib::usize;
using kernel::lib::strcpy, kernel::lib::strlen;
using kernel::lib::Vector;

namespace kernel::fs::tmpfs {

namespace {

// In order to deduce parent nodes automatically, we need the root node to be
// accessible here
Node *root = nullptr;

} /* anonymous namespace */

Node *create_node(NodeType type, const String &path, Node *parent)
{
        // If root does not exist, then create it and return it (so the node
        // we wanted to create would not be created)
        if (!root) {
                root            = new Node;
                root->parent    = nullptr;
                root->path      = "/";
                root->type      = NodeType::Dir;
                root->dir_data  = new Dir;
                root->file_data = nullptr;
                return root;
        }

        Node *nd = new Node;

        Vector<String> parts = parse_path(path);
        String name = parts[parts.size() - 1];

        // The parent node must be a directory because a file cannot store
        // nodes.
        // The parent node can be `nullptr`. In this case, it is automatically
        // deduced using the path parts.
        if (!parent) {
                if (parts.size() <= 1) {
                        parent = root;
                } else {
                        // get the parent path by concatenating every segment
                        // excepted the last one (the name of the node we want
                        // to create)
                        String parent_path = "/";
                        for (usize i = 0; i < parts.size() - 1; i++) {
                                if (i > 0)
                                        parent_path += "/";
                                parent_path += parts[i];
                        }

                        parent = get_node(root, parent_path);
                }

                if (!parent)
                        return nullptr;
        }

        nd->parent = parent;
        // "/" + "a" = "/a"
        if (parent->path.length() == 1 && parent->path[0] == '/')
                nd->path = parent->path + name;
        // "/a" + "/" + "b" = "/a/b"
        else
                nd->path = parent->path + "/" + name;
        
        switch (type) {
        case NodeType::File:
                nd->file_data = new File;
                nd->file_data->data = nullptr;
                nd->dir_data = nullptr;
                break;

        case NodeType::Dir:
                nd->file_data = nullptr;
                nd->dir_data = new Dir;
                break;
        }

        nd->type = type;

        if (parent && parent->dir_data)
                parent->dir_data->nodes.push_back(nd);

        return nd;
}

void remove_node(Node *node)
{
        if (!node)
                return;

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
}

Node *get_node(Node *base, const lib::String &path)
{
        if (base->path == path)
                return base;

        if (base->type != NodeType::Dir)
                return nullptr;

        for (usize i = 0; i < base->dir_data->nodes.size(); i++) {
                Node *child = get_node(base->dir_data->nodes[i], path);
                if (child)
                        return child;
        }

        return nullptr;
}

void write_file(Node *base, const lib::String &path, const char *buf)
{
        Node *f = get_node(base, path);
        if (!f || !f->file_data)
                return;         // file not found
        
        if (f->file_data->data)
                delete[] f->file_data->data;

        usize buf_length = strlen(buf);
        f->file_data->data = new char[buf_length + 1];
        f->file_data->size = buf_length;

        strcpy(buf, f->file_data->data);
}

void read_file(Node *base, const lib::String &path, char *buf, usize n)
{
        Node *f = get_node(base, path);
        if (!f || !f->file_data || !f->file_data->data)
                return;

        if (f->file_data->size >= n)
                return;

        strcpy(f->file_data->data, buf);
}

} /* namespace kernel::fs::tmpfs */