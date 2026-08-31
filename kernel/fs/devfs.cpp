#include <drivers/keyboard.hpp>
#include <fs/devfs.hpp>
#include <fs/vfs.hpp>
#include <lib/memory.hpp>
#include <lib/print.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>

namespace Kiwi::Fs::Devfs
{
        namespace
        {
                enum class NodeType
                {
                        Root,
                        Device
                };

                class DevfsFile : public Vfs::File
                {
                public:
                        class Node *owner;

                        Vfs::Status read(void *buf, Lib::usize size) override;
                        Vfs::Status write(const void *buf, Lib::usize size) override;
                        Vfs::Status close() override;
                };

                class Node : public Vfs::VNode
                {
                public:
                        Lib::String name;
                        NodeType type;
                        DeviceType devtype;
                        DevfsFile *file_data;

                        Vfs::Status mkfile(const Lib::String &name) override;
                        Vfs::Status rm() override;
                        Vfs::Status readdir(Vfs::DirEntry *entry, Lib::usize index) override;
                        Vfs::VNode *lookup(const Lib::String &name) override;
                        Vfs::Status getdirentn(Lib::usize *buf) override;
                        Vfs::File *open() override;
                };

                Node *root = nullptr;
                Lib::Vector<Node *> devices;
        } // anonymous namespace

        Vfs::Status DevfsFile::read(void *buf, Lib::usize n)
        {
                if (not this->owner or this->owner->type != NodeType::Device)
                        return Vfs::Status::NullNode;

                switch (this->owner->devtype) {
                case DeviceType::Input: {
                        for (Lib::usize i = 0; i < n; i++) {
                                char ch = 0;
                                while (not ch)
                                        ch = Drivers::Keyboard::read();

                                static_cast<char *>(buf)[this->cursor + i] = ch;
                        }

                        break;
                }

                default:
                        break;
                }

                return Vfs::Status::Success;
        }

        Vfs::Status DevfsFile::write(const void *buf, Lib::usize n)
        {
                if (not this->owner or this->owner->type != NodeType::Device)
                        return Vfs::Status::NullNode;

                switch (this->owner->devtype) {
                case DeviceType::Console:
                        for (Lib::usize i = 0; i < n; i++)
                                Lib::putchar(static_cast<const char *>(buf)[i]);
                        break;

                default:
                        break;
                }

                return Vfs::Status::Success;
        }

        Vfs::Status DevfsFile::close()
        {
                if (this->owner)
                        this->owner->file_data = nullptr;

                return Vfs::Status::Success;
        }

        Vfs::Status Node::mkfile(const Lib::String &name)
        {
                if (this->type != NodeType::Root)
                        return Vfs::Status::IsADirectory;

                Node *nd        = new Node;
                nd->type        = NodeType::Device;
                nd->devtype     = DeviceType::None;
                nd->name        = name;
                nd->file_data   = nullptr;
                nd->ref_count   = 1;

                devices.pushBack(nd);

                return Vfs::Status::Success;
        }

        Vfs::Status Node::rm()
        {
                if (this->type != NodeType::Device)
                        return Vfs::Status::IsADirectory;

                for (Lib::usize i = 0; i < devices.size(); i++) {
                        if (devices[i] == this) {
                                devices.erase(i);
                                break;
                        }
                }

                if (this->file_data)
                        delete this->file_data;
                delete this;

                return Vfs::Status::Success;
        }

        Vfs::Status Node::readdir(Vfs::DirEntry *entry, Lib::usize n)
        {
                if (this->type != NodeType::Root)
                        return Vfs::Status::IsADirectory;
                if (n >= devices.size())
                        return Vfs::Status::OutOfBounds;

                entry->name = devices[n]->name;
                entry->type = Vfs::DirEntryType::File;

                return Vfs::Status::Success;
        }

        Vfs::VNode *Node::lookup(const Lib::String &name)
        {
                if (this->type != NodeType::Root)
                        return nullptr; // cannot lookup from a device

                if (name == "/")
                        return root;

                for (Lib::usize i = 0; i < devices.size(); i++) {
                        if (devices[i]->name == name)
                                return devices[i];
                }

                return nullptr; // node not found
        }

        Vfs::Status Node::getdirentn(Lib::usize *buf)
        {
                if (this->type != NodeType::Root)
                        return Vfs::Status::NotADirectory;

                Lib::usize count = devices.size();
                Lib::memcpy(buf, &count, sizeof(*buf));

                return Vfs::Status::Success;
        }

        Vfs::File *Node::open()
        {
                if (this->type != NodeType::Device)
                        return nullptr; // can only open device files

                if (not this->file_data) {
                        this->file_data = new DevfsFile;
                        this->file_data->owner = this;
                }

                return this->file_data;
        }

        Vfs::VNode *Devfs::getRoot()
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

        Vfs::Status Devfs::unmount()
        {
                for (Lib::usize i = 0; i < devices.size(); i++) {
                        if (devices[i]->file_data)
                                delete devices[i]->file_data;
                        delete devices[i];
                }

                delete root;
                root = nullptr;

                return Vfs::Status::Success;
        }

        void registerDevice(DeviceType type, const Lib::String &path)
        {
                Vfs::mkfile(path);
                Lib::String devname = path.sub(3);
                Node *dev = reinterpret_cast<Node *>(root->lookup(devname));
                if (dev)
                        dev->devtype = type;
        }
} // namespace Kiwi::Fs::Devfs