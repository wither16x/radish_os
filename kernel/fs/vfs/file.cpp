#include <fs/vfs/file.hpp>
#include <fs/vfs/drive.hpp>
#include <fs/path.hpp>
#include <proc/scheduler.hpp>

namespace Kiwi::Fs::Vfs
{
        Status File::seek(this File &self, Lib::usize cursor, SeekOrigin whence)
        {
                switch (whence) {
                case SeekOrigin::Begin:
                        self.cursor = 0;

                case SeekOrigin::Curr:
                        break;

                case SeekOrigin::End:
                        self.cursor = self.size;
                        break;
                }

                self.cursor += cursor;
                return Status::Success;
        }

        File *openFile(const Lib::String<> &path)
        {
                Node *vnd = lookupNode(path);
                if (not vnd)
                        return nullptr; // node not found

                File *f = vnd->open();
                if (not f) {
                        releaseNode(vnd);
                        return nullptr;
                }
                f->vnode = vnd;
                ++f->ref_count;

                Proc::Process *curr_proc = Proc::Scheduler::getCurrentProcess();
                if (curr_proc)
                        curr_proc->addFileDescriptor(f);

                return f;
        }

        Status closeFile(File *file)
        {
                if (not file)
                        return Status::NullFile;

                Proc::Process *curr_proc = Proc::Scheduler::getCurrentProcess();
                if (curr_proc)
                        curr_proc->removeFileDescriptor(file);

                --file->ref_count;
                if (file->ref_count == 0) {
                        file->close();
                        releaseNode(file->vnode);
                        delete file;
                }

                return Status::Success;
        }

        Status mkfile(const Lib::String<> &path)
        {
                if (not canPathHandleDrive(path))
                        return Status::PathTooShort;

                drive_id_t drive = path[0];
                Lib::String<> rel = path.subString(2);
                Drive &drv = getDriveById(drive);
                if (not drv.root)
                        return Status::NullRoot;
                Lib::Vector<Lib::String<>> parts = parsePath(rel);
                if (parts.isEmpty())
                        return Status::EmptyPath;

                Node *vnd = getNodesFromDriveRoot(drv, parts);

                Status ret = vnd->mkfile(parts[parts.length() - 1]);
                if (vnd != drv.root)
                        releaseNode(vnd);

                return ret;
        }

        Status mkdir(const Lib::String<> &path)
        {
                if (not canPathHandleDrive(path))
                        return Status::PathTooShort;

                drive_id_t drive = path[0];
                Lib::String<> rel = path.subString(2);
                Drive &drv = getDriveById(drive);
                if (not drv.root)
                        return Status::NullRoot;
                Lib::Vector<Lib::String<>> parts = parsePath(rel);
                if (parts.isEmpty())
                        return Status::EmptyPath;

                Node *vnd = getNodesFromDriveRoot(drv, parts);
                if (not vnd)
                        return Status::NullNode;

                Status ret = vnd->mkdir(parts[parts.length() - 1]);
                if (vnd != drv.root)
                        releaseNode(vnd);
                
                return ret;
        }

        Status remove(const Lib::String<> &path)
        {
                Node *vnd = lookupNode(path);
                if (not vnd)
                        return Status::NullNode;

                Status ret = vnd->rm();
                releaseNode(vnd);
                return ret;
        }

        Status write(File *file, const void *buf, Lib::usize size)
        {
                if (not file)
                        return Status::NullFile;

                return file->write(buf, size);
        }

        Status read(File *file, void *buf, Lib::usize size)
        {
                if (not file)
                        return Status::NullFile;

                Status ret = file->read(buf, size);
                return ret;
        }

        Status getfilesz(const Lib::String<> &path, Lib::usize *buf)
        {
                Node *vnd = lookupNode(path);
                if (not vnd)
                        return Status::NullNode;

                Status ret = vnd->getfilesz(buf);
                releaseNode(vnd);
                return ret;
        }
} // namespace Kiwi::Fs::Vfs