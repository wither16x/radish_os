#include <fs/vfs.hpp>
#include <lib/filesystem.hpp>

namespace kernel::lib {

// --------------------------------------------------
int touch(const String &path)
{
        return fs::vfs::touch(path);
}
// --------------------------------------------------

// --------------------------------------------------
int mkdir(const String &path)
{
        return fs::vfs::mkdir(path);
}
// --------------------------------------------------

// --------------------------------------------------
int remove(const String &path)
{
        return fs::vfs::remove(path);
}
// --------------------------------------------------

// --------------------------------------------------
int write(const String &path, const char *buf, usize n)
{
        return fs::vfs::write(path, buf, n);
}
// --------------------------------------------------

// --------------------------------------------------
int read(const String &path, char *buf, usize n)
{
        return fs::vfs::read(path, buf, n);
}
// --------------------------------------------------

// --------------------------------------------------
int getdirent(const String &path, fs::vfs::DirEntry *entry, usize n)
{
        return fs::vfs::readdir(path, entry, n);
}
// --------------------------------------------------

// --------------------------------------------------
fs::vfs::VNode *lookup(const String &path)
{
        return fs::vfs::lookup(path);
}
// --------------------------------------------------

// --------------------------------------------------
int getfilesz(const String &path, usize *buf)
{
        return fs::vfs::getfilesz(path, buf);
}
// --------------------------------------------------

// --------------------------------------------------
int getdirentn(const String &path, usize *buf)
{
        return fs::vfs::getdirentn(path, buf);
}
// --------------------------------------------------

} /* namespace kernel::lib */