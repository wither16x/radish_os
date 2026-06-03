#include <fs/path.hpp>
#include <fs/tmpfs.hpp>
#include <lib/memory.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>

using kernel::lib::String;
using kernel::lib::usize;
using kernel::lib::strcpy, kernel::lib::strlen;

namespace kernel::fs::tmpfs {

Node *create_node(NodeType type, const String &path, Node *parent)
{
        Path p(path);
        p.parse();

        Node *nd = new Node;

        if (parent) {
                nd->parent = parent;
                if (parent->path.length() == 1 && parent->path[0] == '/')
                        nd->path = parent->path + p.get();
                else
                        nd->path = parent->path + '/' + p.get();
        } else {
                nd->parent = nullptr;
                nd->path = p.get();
        }
        
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