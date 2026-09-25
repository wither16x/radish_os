#pragma once

#include <fs/vfs/status.hpp>
#include <fs/vfs/dir.hpp>
#include <fs/vfs/decls.hpp>
#include <lib/typing.hpp>
#include <lib/string.hpp>

namespace Kiwi::Fs::Vfs
{
        struct Node
        {
                virtual ~Node() = default;

                Lib::usize ref_count = 0;

                virtual File *open();
                virtual Status mkfile(const Lib::String<> &name);
                virtual Status mkdir(const Lib::String<> &name);
                virtual Status rm();
                virtual Status getdirentn(Lib::usize *buf);
                virtual Status readdir(DirEntry *entry, Lib::usize index);
                virtual Status getfilesz(Lib::usize *buf);
                virtual Node *lookup(const Lib::String<> &name);
        };

        Node *lookupNode(const Lib::String<> &path);
        Status releaseNode(Node *vnode);
} // namespace Kiwi::Fs::Vfs