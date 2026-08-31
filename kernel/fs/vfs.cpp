#include "lib/typing.hpp"
#include <fs/vfs.hpp>
#include <fs/path.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>

namespace Kiwi::Fs::Vfs
{
        namespace
        {
                /// From A to Z
                constexpr unsigned int MAX_DRIVES = 26;

                struct VFSContext
                {
                        Drive drives[MAX_DRIVES];

                        Drive &getDriveById(this VFSContext &self, drive_id id)
                        {
                                return self.drives[id - 'A'];
                        }
                };

                VFSContext ctx;

                constexpr bool canPathHandleDrive(const Lib::String &path)
                {
                        return path.length() >= 3;
                }

                VNode *getNodesFromDriveRoot(const Drive &drv, const Lib::Vector<Lib::String> &parts)
                {
                        VNode *curr_nd = drv.root;
                        for (Lib::usize i = 0; i < parts.size() - 1; i++) {
                                VNode *child = curr_nd->lookup(parts[i]);
                                if (not child)
                                        return nullptr;

                                curr_nd = static_cast<VNode *>(child);
                        }

                        return curr_nd;
                }
        } // anonymous namespace

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

        Status mount(drive_id drive, FileSystem *fs)
        {
                Drive &drv = ctx.getDriveById(drive);
                if (drv.fs)
                        return Status::FsMounted;

                drv.id = drive;
                drv.fs = fs;
                drv.root = fs->getRoot();
                if (drv.root)
                        ++drv.root->ref_count;

                return Status::Success;
        }

        Status unmount(drive_id drive)
        {
                Drive &drv = ctx.getDriveById(drive);
                if (not drv.fs)
                        return Status::FsNotMounted;

                drv.fs->unmount();
                delete drv.fs;
                drv.fs = nullptr;
                releaseNode(drv.root);
                drv.root = nullptr;

                return Status::Success;
        }

        VNode *lookupNode(const Lib::String &path)
        {
                if (not canPathHandleDrive(path))
                        return nullptr;

                drive_id drive = path[0];
                Lib::String rel = path.sub(2);

                Drive &drv = ctx.getDriveById(drive);

                Lib::Vector<Lib::String> parts = parsePath(rel);

                VNode *curr_nd = drv.root;
                ++curr_nd->ref_count;

                for (Lib::usize i = 0; i < parts.size(); i++) {
                        void *child = curr_nd->lookup(parts[i]);
                        if (not child) {
                                releaseNode(curr_nd);
                                return nullptr;         // child not found
                        }

                        VNode *next = static_cast<VNode *>(child);
                        ++next->ref_count;

                        releaseNode(curr_nd);

                        curr_nd = next;
                }

                return curr_nd;
        }

        Status releaseNode(VNode *vnd)
        {
                if (not vnd)
                        return Status::NullNode;
                if (vnd->ref_count == 0)
                        return Status::NoRefs;

                --vnd->ref_count;
                if (vnd->ref_count == 0)
                        delete vnd;

                return Status::Success;
        }

        File *openFile(const Lib::String &path)
        {
                VNode *vnd = lookupNode(path);
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

        Status mkfile(const Lib::String &path)
        {
                if (not canPathHandleDrive(path))
                        return Status::PathTooShort;

                drive_id drive = path[0];
                Lib::String rel = path.sub(2);
                Drive &drv = ctx.getDriveById(drive);
                if (not drv.root)
                        return Status::NullRoot;
                Lib::Vector<Lib::String> parts = parsePath(rel);
                if (parts.empty())
                        return Status::EmptyPath;

                VNode *vnd = getNodesFromDriveRoot(drv, parts);

                Status ret = vnd->mkfile(parts[parts.size() - 1]);
                if (vnd != drv.root)
                        releaseNode(vnd);

                return ret;
        }

        Status mkdir(const Lib::String &path)
        {
                if (not canPathHandleDrive(path))
                        return Status::PathTooShort;

                drive_id drive = path[0];
                Lib::String rel = path.sub(2);
                Drive &drv = ctx.getDriveById(drive);
                if (not drv.root)
                        return Status::NullRoot;
                Lib::Vector<Lib::String> parts = parsePath(rel);
                if (parts.empty())
                        return Status::EmptyPath;

                VNode *vnd = getNodesFromDriveRoot(drv, parts);
                if (not vnd)
                        return Status::NullNode;

                Status ret = vnd->mkdir(parts[parts.size() - 1]);
                if (vnd != drv.root)
                        releaseNode(vnd);
                
                return ret;
        }

        Status remove(const Lib::String &path)
        {
                VNode *vnd = lookupNode(path);
                if (!vnd)
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

        Status readdir(const Lib::String &path, DirEntry *entry, Lib::usize index)
        {
                VNode *vnd = lookupNode(path);
                if (not vnd)
                        return Status::NullNode;

                Status ret = vnd->readdir(entry, index);
                releaseNode(vnd);
                return ret;
        }

        Status getfilesz(const Lib::String &path, Lib::usize *buf)
        {
                VNode *vnd = lookupNode(path);
                if (not vnd)
                        return Status::NullNode;

                Status ret = vnd->getfilesz(buf);
                releaseNode(vnd);
                return ret;
        }

        Status getdirentn(const Lib::String &path, Lib::usize *buf)
        {
                VNode *vnd = lookupNode(path);
                if (not vnd)
                        return Status::NullNode;
                
                Status ret = vnd->getdirentn(buf);
                releaseNode(vnd);
                return ret;
        }
} // namespace Kiwi::Fs::Vfs