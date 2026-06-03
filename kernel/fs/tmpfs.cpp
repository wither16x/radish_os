// please do not read this file
// I'm ashamed of it
// gonna improve it
// later...

#include "lib/logging.hpp"
#include <fs/path.hpp>
#include <fs/tmpfs.hpp>
#include <lib/string.hpp>

using kernel::lib::String;

namespace kernel::fs::tmpfs {

Node *create_node(NodeType type, const String &path, Node *parent)
{
        Path p(path);
        p.parse();

        Node *nd = new Node;
        if (!nd)
                return nullptr;

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
        lib::log::logger.debug("nd->path = %s", nd->path.raw());
        
        switch (type) {
        case NodeType::File:
                nd->file_data = new File;
                if (!nd->file_data)
                        return nullptr;
                nd->dir_data = nullptr;
                break;

        case NodeType::Dir:
                nd->file_data = nullptr;
                nd->dir_data = new Dir;
                if (!nd->dir_data)
                        return nullptr;
                break;
        }

        return nd;
}

void remove_node(Node *node)
{
        if (node)
                delete node;
        if (node && node->file_data)
                delete node->file_data;
        if (node && node->dir_data)
                delete node->dir_data;
}

} /* namespace kernel::fs::tmpfs */