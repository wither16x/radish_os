#pragma once

#include <lib/string.hpp>
#include <fs/vfs/status.hpp>

namespace Kiwi::Fs::Vfs
{
        enum class DirEntryType
        {
                File,
                Dir
        };

        struct DirEntry
        {
                Lib::String<> name;
                DirEntryType type;
        };

        Status readdir(const Lib::String<> &path, DirEntry *entry, Lib::usize index);
        Status getdirentn(const Lib::String<> &path, Lib::usize *buf);
} // namespace Kiwi::Fs::Vfs