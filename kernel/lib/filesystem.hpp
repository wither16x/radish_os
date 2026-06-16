#pragma once

#include <fs/vfs.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>

namespace kernel::lib {

int create_file(const String &path);
int create_dir(const String &path);
int remove(const String &path);
int write_file(const String &path, const char *buf, usize n);
int read_file(const String &path, char *buf, usize n);
int getdirent(const String &path, fs::vfs::DirEntry *entry, usize n);
fs::vfs::VNode *lookup(const String &path);
int getfilesz(const String &path, usize *buf);
int getdirentn(const String &path, usize *buf);

} /* namespace kernel::lib */