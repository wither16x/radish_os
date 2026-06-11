#pragma once

#include <fs/vfs.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>

namespace kernel::lib {

int create_file(const String &path);
int create_dir(const String &path);
int remove(const String &path);
int write_file(const String &path, const char *buf, usize n);
int read_file(const String &path, char *buf, usize n);
int readdir(const String &path, Vector<fs::vfs::DirEntry> &entries);
fs::vfs::VNode *lookup(const String &path);
usize get_file_size(const String &path);

} /* namespace kernel::lib */