#include <fs/vfs/dir.hpp>
#include <fs/vfs/node.hpp>

namespace Kiwi::Fs::Vfs
{
        Status readdir(const Lib::String<> &path, DirEntry *entry, Lib::usize index)
        {
                Node *vnd = lookupNode(path);
                if (not vnd)
                        return Status::NullNode;

                Status ret = vnd->readdir(entry, index);
                releaseNode(vnd);
                return ret;
        }

        Status getdirentn(const Lib::String<> &path, Lib::usize *buf)
        {
                Node *vnd = lookupNode(path);
                if (not vnd)
                        return Status::NullNode;
                
                Status ret = vnd->getdirentn(buf);
                releaseNode(vnd);
                return ret;
        }
} // namespace Kiwi::Fs::Vfs