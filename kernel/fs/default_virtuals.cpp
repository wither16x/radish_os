#include <fs/vfs.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Fs::Vfs
{
        File *VNode::open()
        {
                return nullptr;
        }

        Status VNode::mkfile(const Lib::String &name)
        {
                static_cast<void>(name);
                return Status::NotImplemented;
        }

        Status VNode::mkdir(const Lib::String &name)
        {
                static_cast<void>(name);
                return Status::NotImplemented;
        }

        Status VNode::rm()
        {
                return Status::NotImplemented;
        }

        Status VNode::getdirentn(Lib::usize *buf)
        {
                static_cast<void>(buf);
                return Status::NotImplemented;
        }

        Status VNode::readdir(DirEntry *entry, Lib::usize index)
        {
                static_cast<void>(entry);
                static_cast<void>(index);
                return Status::NotImplemented;
        }

        Status VNode::getfilesz(Lib::usize *buf)
        {
                static_cast<void>(buf);
                return Status::NotImplemented;
        }

        VNode *VNode::lookup(const Lib::String &name)
        {
                static_cast<void>(name);
                return nullptr;
        }

        Status File::write(const void *buf, Lib::usize size)
        {
                static_cast<void>(buf);
                static_cast<void>(size);
                return Status::NotImplemented;
        }

        Status File::read(void *buf, Lib::usize size)
        {
                static_cast<void>(buf);
                static_cast<void>(size);
                return Status::NotImplemented;
        }

        Status File::close()
        {
                return Status::NotImplemented;       
        }
} // namespace Kiwi::Fs::Vfs