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

class DevfsFile : public vfs::File {
public:
        class Node *owner;

        vfs::Status read(void *buf, usize size) override;
        vfs::Status write(const void *buf, usize size) override;
        vfs::Status close() override;
};

class Node : public vfs::VNode {
public:
        String name;
        NodeType type;
        DeviceType devtype;

        DevfsFile *file_data;

        vfs::Status mkfile(const String &name) override;
        vfs::Status rm() override;
        vfs::Status readdir(vfs::DirEntry *entry, usize index) override;
        vfs::VNode *lookup(const String &name) override;
        vfs::Status getdirentn(usize *buf) override;
        vfs::File *open() override;
};

Node *root = nullptr;
Vector<Node *> devices;

} /* anonymous namespace */

vfs::Status DevfsFile::read(void *buf, usize n)
{
        if (not this->owner or this->owner->type != NodeType::Device)
                return vfs::Status::NullNode;

        switch (this->owner->devtype) {
        case DeviceType::Input: {
                for (usize i = 0; i < n; i++) {
                        char ch = 0;
                        while (not ch)
                                ch = drivers::keyboard::read();

                        static_cast<char *>(buf)[i] = ch;
                }

                break;
        }

        default:
                break;
        }

        return vfs::Status::Success;
}

vfs::Status DevfsFile::write(const void *buf, usize n)
{
        if (not this->owner or this->owner->type != NodeType::Device)
                return vfs::Status::NullNode;

        switch (this->owner->devtype) {
        case DeviceType::Console:
                for (usize i = 0; i < n; i++)
                        putchar(static_cast<const char *>(buf)[i]);
                break;

        default:
                break;
        }

        return vfs::Status::Success;
}

vfs::Status DevfsFile::close()
{
        if (this->owner)
                this->owner->file_data = nullptr;

        return vfs::Status::Success;
}

vfs::Status Node::mkfile(const String &name)
{
        if (this->type != NodeType::Root)
                return vfs::Status::IsADirectory;

        Node *nd        = new Node;
        nd->type        = NodeType::Device;
        nd->devtype     = DeviceType::None;
        nd->name        = name;
        nd->file_data   = nullptr;
        nd->ref_count   = 1;

        devices.push_back(nd);

        return vfs::Status::Success;
}

vfs::Status Node::rm()
{
        if (this->type != NodeType::Device)
                return vfs::Status::IsADirectory;

        for (usize i = 0; i < devices.size(); i++) {
                if (devices[i] == this) {
                        devices.erase(i);
                        break;
                }
        }

        if (this->file_data)
                delete this->file_data;
        delete this;

        return vfs::Status::Success;
}

vfs::Status Node::readdir(vfs::DirEntry *entry, usize n)
{
        if (this->type != NodeType::Root)
                return vfs::Status::IsADirectory;
        if (n >= devices.size())
                return vfs::Status::OutOfBounds;

        entry->name = devices[n]->name;
        entry->type = vfs::DirEntryType::File;

        return vfs::Status::Success;
}

vfs::VNode *Node::lookup(const String &name)
{
        if (this->type != NodeType::Root)
                return nullptr; // cannot lookup from a device

        if (name == "/")
                return root;

        for (usize i = 0; i < devices.size(); i++) {
                if (devices[i]->name == name)
                        return devices[i];
        }

        return nullptr; // node not found
}

vfs::Status Node::getdirentn(usize *buf)
{
        if (this->type != NodeType::Root)
                return vfs::Status::NotADirectory;

        usize count = devices.size();
        memcpy(buf, &count, sizeof(*buf));

        return vfs::Status::Success;
}

vfs::File *Node::open()
{
        if (this->type != NodeType::Device)
                return nullptr; // can only open device files

        if (not this->file_data) {
                this->file_data = new DevfsFile;
                this->file_data->owner = this;
        }

        return this->file_data;
}

vfs::VNode *DEVFS::get_root()
{
        if (not root) {
                root            = new Node;
                root->name      = "/";
                root->type      = NodeType::Root;
                root->devtype   = DeviceType::None;
                root->file_data = nullptr;
                root->ref_count = 1;
        }

        return root;
}

vfs::Status DEVFS::unmount()
{
        for (usize i = 0; i < devices.size(); i++) {
                if (devices[i]->file_data)
                        delete devices[i]->file_data;
                delete devices[i];
        }

        delete root;
        root = nullptr;

        return vfs::Status::Success;
}

void register_device(DeviceType type, const lib::String &path)
{
        vfs::mkfile(path);
        String devname = path.sub(3);
        Node *dev = reinterpret_cast<Node *>(root->lookup(devname));
        if (dev)
                dev->devtype = type;
}

} /* namespace kernel::fs::devfs */