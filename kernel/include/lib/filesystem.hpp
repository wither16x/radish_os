#pragma once

#include <fs/vfs.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Lib
{
        using File = Fs::Vfs::File;

        File *open(const String &path);
        Fs::Vfs::Status close(File *f);
        /// Create a new file.
        Fs::Vfs::Status mkfile(const String &path);
        /// Create a new directory.
        Fs::Vfs::Status mkdir(const String &path);
        Fs::Vfs::Status write(File *f, const void *buf, usize n);
        Fs::Vfs::Status read(File *f, void *buf, usize n);
        /// Remove a file or a directory.
        Fs::Vfs::Status rm(const String &path);
        /// Get a single entry from a directory. 
        Fs::Vfs::Status getdirent(const String &path, Fs::Vfs::DirEntry *entry, usize n);
        /// Get the size of a file.
        Fs::Vfs::Status getfilesz(const String &path, usize *buf);
        /// Get the number of entries of a directory.
        Fs::Vfs::Status getdirentn(const String &path, usize *buf);
} // namespace Kiwi::Lib