#include <fs/vfs.hpp>
#include <lib/filesystem.hpp>

namespace Kiwi::Lib
{
        File *open(const String &path)
        {
                return Fs::Vfs::openFile(path);
        }

        Fs::Vfs::Status close(File *f)
        {
                return Fs::Vfs::closeFile(f);
        }

        Fs::Vfs::Status mkfile(const String &path)
        {
                return Fs::Vfs::mkfile(path);
        }

        Fs::Vfs::Status mkdir(const String &path)
        {
                return Fs::Vfs::mkdir(path);
        }

        Fs::Vfs::Status rm(const String &path)
        {
                return Fs::Vfs::remove(path);
        }

        Fs::Vfs::Status write(File *f, const void *buf, usize n)
        {
                return Fs::Vfs::write(f, buf, n);
        }

        Fs::Vfs::Status read(File *f, void *buf, usize n)
        {
                return Fs::Vfs::read(f, buf, n);
        }

        Fs::Vfs::Status getdirent(const String &path, Fs::Vfs::DirEntry *entry, usize n)
        {
                return Fs::Vfs::readdir(path, entry, n);
        }

        Fs::Vfs::Status getfilesz(const String &path, usize *buf)
        {
                return Fs::Vfs::getfilesz(path, buf);
        }

        Fs::Vfs::Status getdirentn(const String &path, usize *buf)
        {
                return Fs::Vfs::getdirentn(path, buf);
        }
} // namespace Kiwi::Lib