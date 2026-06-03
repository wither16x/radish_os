// please do not read this file
// I'm ashamed of it
// gonna improve it
// later...

#include <fs/path.hpp>
#include <fs/tmpfs.hpp>
#include <lib/string.hpp>

using kernel::lib::String;

namespace kernel::fs::tmpfs {

Node *create_node(NodeType type, const String &path)
{
        Path p(path);
        if (!p.is_valid())
                return nullptr;
        p.parse();

        Node *nd = new Node;
        if (!nd)
                return nullptr;

        nd->path = p.get();
        
        switch (type) {
        case NodeType::File:
                nd->file_data = new File;
                if (!nd->file_data)
                        return nullptr;

                nd->dir_data = nullptr;

        case NodeType::Dir:
                nd->file_data = nullptr;
                nd->dir_data = new Dir;
                if (!nd->dir_data)
                        return nullptr;
        }

        return nd;
}

} /* namespace kernel::fs::tmpfs */