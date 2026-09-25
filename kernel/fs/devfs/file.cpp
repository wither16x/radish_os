#include <fs/devfs/file.hpp>
#include <drivers/keyboard.hpp>
#include <lib/print.hpp>

namespace Kiwi::Fs::Devfs
{
        Vfs::Status File::read(void *buf, Lib::usize n)
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

        Vfs::Status File::write(const void *buf, Lib::usize n)
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

        Vfs::Status File::close()
        {
                if (this->owner)
                        this->owner->file_data = nullptr;

                return Vfs::Status::Success;
        }

        Vfs::Status Node::mkfile(const Lib::String<> &name)
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

                for (Lib::usize i = 0; i < devices.length(); i++) {
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
} // namespace Kiwi::Fs::Devfs