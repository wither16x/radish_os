#include <fs/vfs.hpp>
#include <lib/filesystem.hpp>

namespace kernel::lib {

File *open(const String &path)
{
        return fs::vfs::open_file(path);
}

fs::vfs::Status close(File *f)
{
        return fs::vfs::close_file(f);
}

fs::vfs::Status mkfile(const String &path)
{
        return fs::vfs::mkfile(path);
}

// --------------------------------------------------
fs::vfs::Status mkdir(const String &path)
{
        return fs::vfs::mkdir(path);
}
// --------------------------------------------------

// --------------------------------------------------
fs::vfs::Status remove(const String &path)
{
        return fs::vfs::remove(path);
}
// --------------------------------------------------

// --------------------------------------------------
fs::vfs::Status write(File *f, const void *buf, usize n)
{
        return fs::vfs::write(f, buf, n);
}
// --------------------------------------------------

// --------------------------------------------------
fs::vfs::Status read(File *f, void *buf, usize n)
{
        return fs::vfs::read(f, buf, n);
}
// --------------------------------------------------

// --------------------------------------------------
fs::vfs::Status getdirent(const String &path, fs::vfs::DirEntry *entry, usize n)
{
        return fs::vfs::readdir(path, entry, n);
}
// --------------------------------------------------

// --------------------------------------------------
fs::vfs::Status getfilesz(const String &path, usize *buf)
{
        return fs::vfs::getfilesz(path, buf);
}
// --------------------------------------------------

// --------------------------------------------------
fs::vfs::Status getdirentn(const String &path, usize *buf)
{
        return fs::vfs::getdirentn(path, buf);
}
// --------------------------------------------------

} /* namespace kernel::lib */