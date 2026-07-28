#pragma once

#include <fs/vfs.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>

namespace kernel::lib {

using File = fs::vfs::File;

File *open(const String &path);
fs::vfs::Status close(File *f);
/// Create a new file.
fs::vfs::Status mkfile(const String &path);
/// Create a new directory.
fs::vfs::Status mkdir(const String &path);
/// Remove a file or a directory.
fs::vfs::Status rm(const String &path);
/// Write bytes to a file.
fs::vfs::Status write(File *f, const void *buf, usize n);
/// Read bytes from a file.
fs::vfs::Status read(File *f, void *buf, usize n);
/// Get a single entry from a directory. 
fs::vfs::Status getdirent(const String &path, fs::vfs::DirEntry *entry, usize n);
/// Get the size of a file.
fs::vfs::Status getfilesz(const String &path, usize *buf);
/// Get the number of entries of a directory.
fs::vfs::Status getdirentn(const String &path, usize *buf);

} /* namespace kernel::lib */