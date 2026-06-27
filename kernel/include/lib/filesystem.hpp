#pragma once

#include <fs/vfs.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>

namespace kernel::lib {

/// Create a new file.
int create_file(const String &path);
/// Create a new directory. 
int create_dir(const String &path);
/// Remove a file or a directory.
int remove(const String &path);
/// Write bytes to a file.
int write_file(const String &path, const char *buf, usize n);
/// Read bytes from a file.
int read_file(const String &path, char *buf, usize n);
/// Get a single entry from a directory. 
int getdirent(const String &path, fs::vfs::DirEntry *entry, usize n);
/// Lookup a file.
fs::vfs::VNode *lookup(const String &path);
/// Get the size of a file.
int getfilesz(const String &path, usize *buf);
/// Get the number of entries of a directory.
int getdirentn(const String &path, usize *buf);

} /* namespace kernel::lib */