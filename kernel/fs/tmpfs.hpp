// please do not read this file
// I'm ashamed of it
// gonna improve it
// later...

#pragma once

#include <lib/string.hpp>
#include <lib/vector.hpp>

namespace kernel::fs::tmpfs {

constexpr int MaxPathChars = 255;
constexpr int MaxDataChars = 1000;

enum class NodeType : int {
        File,
        Dir
};

// Depending on the node type, `file_data` or `dir_data` is set
// to `nullptr`.
// The parent node may also be set to `nullptr` but only for the first
// node (root directory)!
struct Node {
        Node *parent;

        NodeType type;
        lib::String path;

        struct File *file_data;
        struct Dir *dir_data;
};

struct File {
        lib::String data;
};

struct Dir {
        lib::Vector<Node *> nodes;
};

Node *create_node(NodeType type, const lib::String &path, Node *parent);
void remove_node(Node *node);

} /* namespace kernel::fs::tmpfs */