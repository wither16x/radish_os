#pragma once

#include <fs/vfs/node.hpp>

namespace Kiwi::Fs::Vfs
{
        enum class SeekOrigin
        {
                Begin,
                Curr,
                End
        };

        struct File
        {
                virtual ~File() = default;

                Node *vnode;
                Lib::usize size;
                Lib::usize ref_count = 0;
                Lib::usize cursor = 0;

                virtual Status write(const void *buf, Lib::usize size);
                virtual Status read(void *buf, Lib::usize size);
                virtual Status close();
                
                Status seek(this File &self, Lib::usize pos, SeekOrigin whence);
        };

        File *openFile(const Lib::String<> &path);
        Status closeFile(File *file);
        Status mkfile(const Lib::String<> &path);
        Status mkdir(const Lib::String<> &path);
        Status remove(const Lib::String<> &path);
        Status write(File *file, const void *buf, Lib::usize size);
        Status read(File *file, void *buf, Lib::usize size);
        Status getfilesz(const Lib::String<> &path, Lib::usize *buf);
} // namespace Kiwi::Fs::Vfs