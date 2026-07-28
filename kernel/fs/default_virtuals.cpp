#include <fs/vfs.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>

using kernel::lib::String;
using kernel::lib::usize;

namespace kernel::fs::vfs {

File *VNode::open()
{
        return nullptr;
}

Status VNode::mkfile(const String &name)
{
        static_cast<void>(name);
        return Status::NotImplemented;
}

Status VNode::mkdir(const String &name)
{
        static_cast<void>(name);
        return Status::NotImplemented;
}

Status VNode::rm()
{
        return Status::NotImplemented;
}

Status VNode::getdirentn(usize *buf)
{
        static_cast<void>(buf);
        return Status::NotImplemented;
}

Status VNode::readdir(DirEntry *entry, usize index)
{
        static_cast<void>(entry);
        static_cast<void>(index);
        return Status::NotImplemented;
}

Status VNode::getfilesz(usize *buf)
{
        static_cast<void>(buf);
        return Status::NotImplemented;
}

VNode *VNode::lookup(const String &name)
{
        static_cast<void>(name);
        return nullptr;
}

Status File::write(const void *buf, usize size)
{
        static_cast<void>(buf);
        static_cast<void>(size);
        return Status::NotImplemented;
}

Status File::read(void *buf, usize size)
{
        static_cast<void>(buf);
        static_cast<void>(size);
        return Status::NotImplemented;
}

Status File::close()
{
        return Status::NotImplemented;       
}

} /* namespace kernel::fs::vfs */