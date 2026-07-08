#include <drivers/keyboard.hpp>
#include <fs/devfs.hpp>
#include <fs/vfs.hpp>
#include <lib/memory.hpp>
#include <lib/print.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>

using kernel::lib::memcpy;
using kernel::lib::putchar;
using kernel::lib::String;
using kernel::lib::usize;
using kernel::lib::Vector;

namespace kernel::fs::devfs {

namespace {

enum class NodeType : int {
        Root,
        Device
};

struct Node : public vfs::VNode {
        Node *parent;

        String name;
        NodeType type;
        DeviceType devtype;

        struct Dir *dir_data;

        int touch(const String &name) override;
        int remove() override;
        int read(void *buf, usize n) override;
        int write(const void *buf, usize n) override;
        int readdir(vfs::DirEntry *entry, usize n) override;
        void *lookup(const String &name) override;
        int getdirentn(usize *buf) override;
};

struct Dir {
        Vector<Node *> nodes;
};

Node *root = nullptr;

int remove_node(Node *node)
{
        if (!node)
                return -1;      // node is null

        if (node->dir_data) {
                for (usize i = 0; i < node->dir_data->nodes.size(); i++)
                        remove_node(node->dir_data->nodes[i]);

                delete node->dir_data;
        }

        delete node;

        return 0;
}

} /* anonymous namespace */

int Node::touch(const String &name)
{
        if (this->type != NodeType::Root)
                return -1; /* a device cannot own another device */

        Node *nd = new Node;
        nd->parent = this;
        nd->type = NodeType::Device;
        nd->devtype = DeviceType::None;
        nd->name = name;
        nd->dir_data = nullptr;

        this->dir_data->nodes.push_back(nd);

        return 0;
}

int Node::remove()
{
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

int Node::read(void *buf, usize n)
{
        if (this->type != NodeType::Device)
                return -1; // cannot read like that from the root

        switch (this->devtype) {
        case DeviceType::Input: {
                for (usize i = 0; i < n; i++) {
                        char ch = 0;
                        while (!ch)
                                ch = drivers::keyboard::read();

                        static_cast<char *>(buf)[i] = ch;
                        break;
                }
        }

        default:
                break;
        }

        return 0;
}

int Node::write(const void *buf, usize n)
{
        if (this->type != NodeType::Device)
                return -1;      // cannot write like that in the root

        switch (this->devtype) {
        case DeviceType::Console:
                for (usize i = 0; i < n; i++)
                        putchar(static_cast<const char *>(buf)[i]);
                break;

        default:
                break;
        }

        return 0;
}

int Node::readdir(vfs::DirEntry *entry, usize n)
{
        if (this->type != NodeType::Root)
                return -1;      // node is not the root

        Node *nd = this->dir_data->nodes[n];
        entry->name = nd->name;
        entry->is_dir = nd->type == NodeType::Root;

        return 0;
}

void *Node::lookup(const String &name)
{
        if (this->type != NodeType::Root)
                return nullptr; // cannot lookup from a device

        if (name == "/")
                return root;

        for (usize i = 0; i < this->dir_data->nodes.size(); i++) {
                if (this->dir_data->nodes[i]->name == name)
                        return this->dir_data->nodes[i]; // node found
        }

        return nullptr; // node not found
}

int Node::getdirentn(usize *buf)
{
        if (this->type != NodeType::Root)
                return -1; // node is not the root

        usize count = this->dir_data->nodes.size();
        memcpy(buf, &count, sizeof(*buf));

        return 0;
}

vfs::VNode *DEVFS::get_root()
{
        if (!root) {
                root            = new Node;
                root->parent    = nullptr;
                root->name      = "/";
                root->type      = NodeType::Root;
                root->devtype   = DeviceType::None;
                root->dir_data  = new Dir;
                root->owned     = false;
        }

        return root;
}

void DEVFS::unmount()
{
        root->remove();
        root = nullptr;
}

void register_device(DeviceType type, const lib::String &path)
{
        vfs::touch(path);
        String devname = path.sub(3);
        Node *dev = reinterpret_cast<Node *>(root->lookup(devname));
        dev->devtype = type;
}

} /* namespace kernel::fs::devfs */