#include <fs/vfs.hpp>
#include <lib/filesystem.hpp>

namespace kernel::lib {

int create_file(const String &path)
{
        return fs::vfs::create_file(path);
}

int create_dir(const String &path)
{
        return fs::vfs::create_dir(path);
}

int remove(const String &path)
{
        return fs::vfs::remove(path);
}

int write_file(const String &path, const char *buf, usize n)
{
        return fs::vfs::write_file(path, buf, n);
}

int read_file(const String &path, char *buf, usize n)
{
        return fs::vfs::read_file(path, buf, n);
}

int readdir(const String &path, fs::vfs::DirEntry *entry, usize n)
{
        return fs::vfs::readdir(path, entry, n);
}

fs::vfs::VNode *lookup(const String &path)
{
        return fs::vfs::lookup(path);
}

int getfilesz(const String &path, usize *buf)
{
        return fs::vfs::getfilesz(path, buf);
}

int getdirentn(const String &path, usize *buf)
{
        return fs::vfs::getdirentn(path, buf);
}

} /* namespace kernel::lib */