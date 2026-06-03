#pragma once

#include <lib/string.hpp>
#include <lib/typing.hpp>
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
        char *data;
        lib::usize size;
};

struct Dir {
        lib::Vector<Node *> nodes;
};

Node *create_node(NodeType type, const lib::String &path, Node *parent);
// recursive on `NodeType::Dir`
void remove_node(Node *node);
// `base` is the node from where we want to find the node which corresponds to
// `path`. `base->type` must be equal to `NodeType::Dir`. Otherwise `nullptr`
// will be returned.
Node *get_node(Node *base, const lib::String &path);

void write_file(Node *base, const lib::String &path, const char *buf);
void read_file(Node *base, const lib::String &path, char *buf, lib::usize n);

} /* namespace kernel::fs::tmpfs */