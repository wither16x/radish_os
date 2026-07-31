#include <fs/vfs.hpp>
#include <fs/path.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>

using kernel::lib::String;
using kernel::lib::Vector;
using kernel::lib::usize;

namespace kernel::fs::vfs {

namespace {

/// From A to Z
constexpr unsigned int MAX_DRIVES = 26;

struct VFSContext {
        Drive drives[MAX_DRIVES];

        Drive &get_drive_by_id(this VFSContext &self, drive_id id)
        {
                return self.drives[id - 'A'];
        }
};

VFSContext ctx;

constexpr bool can_path_handle_drive(const String &path)
{
        return path.length() >= 3;
}

VNode *get_nodes_from_drive_root(const Drive &drv, const Vector<String> &parts)
{
        VNode *curr_nd = drv.root;
        for (usize i = 0; i < parts.size() - 1; i++) {
                VNode *child = curr_nd->lookup(parts[i]);
                if (not child)
                        return nullptr;

                curr_nd = static_cast<VNode *>(child);
        }

        return curr_nd;
}

} /* anonymous namespace */

Status mount(drive_id drive, FileSystem *fs)
{
        Drive &drv = ctx.get_drive_by_id(drive);
        if (drv.fs)
                return Status::FsMounted;

        drv.id = drive;
        drv.fs = fs;
        drv.root = fs->get_root();
        if (drv.root)
                ++drv.root->ref_count;

        return Status::Success;
}

Status unmount(drive_id drive)
{
        Drive &drv = ctx.get_drive_by_id(drive);
        if (not drv.fs)
                return Status::FsNotMounted;

        drv.fs->unmount();
        delete drv.fs;
        drv.fs = nullptr;
        release_node(drv.root);
        drv.root = nullptr;

        return Status::Success;
}

VNode *lookup_node(const String &path)
{
        if (not can_path_handle_drive(path))
                return nullptr;

        drive_id drive = path[0];
        String rel = path.sub(2);

        Drive &drv = ctx.get_drive_by_id(drive);

        Vector<String> parts = parse_path(rel);

        VNode *curr_nd = drv.root;
        ++curr_nd->ref_count;

        for (usize i = 0; i < parts.size(); i++) {
                void *child = curr_nd->lookup(parts[i]);
                if (not child) {
                        release_node(curr_nd);
                        return nullptr;         // child not found
                }

                VNode *next = static_cast<VNode *>(child);
                ++next->ref_count;

                release_node(curr_nd);

                curr_nd = next;
        }

        return curr_nd;
}

Status release_node(VNode *vnd)
{
        if (!vnd)
                return Status::NullNode;
        if (vnd->ref_count == 0)
                return Status::NoRefs;

        --vnd->ref_count;
        if (vnd->ref_count == 0)
                delete vnd;

        return Status::Success;
}

File *open_file(const lib::String &path)
{
        VNode *vnd = lookup_node(path);
        if (not vnd)
                return nullptr; // node not found

        File *f = vnd->open();
        if (not f) {
                release_node(vnd);
                return nullptr;
        }
        f->vnode = vnd;
        ++f->ref_count;

        proc::Process *curr_proc = proc::scheduler::get_current_process();
        if (curr_proc)
                curr_proc->add_file_descriptor(f);

        return f;
}

Status close_file(File *file)
{
        if (not file)
                return Status::NullFile; // file descriptor is null

        proc::Process *curr_proc = proc::scheduler::get_current_process();
        if (curr_proc)
                curr_proc->remove_file_descriptor(file);

        if (file->ref_count == 0)
                return Status::NoRefs;

        --file->ref_count;
        if (file->ref_count == 0) {
                file->close();
                release_node(file->vnode);
                delete file;
        }

        return Status::Success;
}

Status mkfile(const lib::String &path)
{
        if (not can_path_handle_drive(path))
                return Status::PathTooShort;

        drive_id drive = path[0];
        String rel = path.sub(2);
        Drive &drv = ctx.get_drive_by_id(drive);
        if (not drv.root)
                return Status::NullRoot;
        Vector<String> parts = parse_path(rel);
        if (parts.empty())
                return Status::EmptyPath;

        VNode *vnd = get_nodes_from_drive_root(drv, parts);

        Status ret = vnd->mkfile(parts[parts.size() - 1]);
        if (vnd != drv.root)
                release_node(vnd);

        return ret;
}

Status mkdir(const lib::String &path)
{
        if (not can_path_handle_drive(path))
                return Status::PathTooShort;

        drive_id drive = path[0];
        String rel = path.sub(2);
        Drive &drv = ctx.get_drive_by_id(drive);
        if (not drv.root)
                return Status::NullRoot;
        Vector<String> parts = parse_path(rel);
        if (parts.empty())
                return Status::EmptyPath;

        VNode *vnd = get_nodes_from_drive_root(drv, parts);
        if (not vnd)
                return Status::NullNode;

        Status ret = vnd->mkdir(parts[parts.size() - 1]);
        if (vnd != drv.root)
                release_node(vnd);
        
        return ret;
}

Status remove(const lib::String &path)
{
        VNode *vnd = lookup_node(path);
        if (!vnd)
                return Status::NullNode;

        Status ret = vnd->rm();
        release_node(vnd);
        return ret;
}

Status write(File *file, const void *buf, usize size)
{
        if (not file)
                return Status::NullFile;

        return file->write(buf, size);
}

Status read(File *file, void *buf, usize size)
{
        if (not file)
                return Status::NullFile;

        Status ret = file->read(buf, size);
        return ret;
}

Status readdir(const String &path, DirEntry *entry, usize index)
{
        VNode *vnd = lookup_node(path);
        if (not vnd)
                return Status::NullNode;

        Status ret = vnd->readdir(entry, index);
        release_node(vnd);
        return ret;
}

Status getfilesz(const lib::String &path, usize *buf)
{
        VNode *vnd = lookup_node(path);
        if (not vnd)
                return Status::NullNode;

        Status ret = vnd->getfilesz(buf);
        release_node(vnd);
        return ret;
}

Status getdirentn(const lib::String &path, lib::usize *buf)
{
        VNode *vnd = lookup_node(path);
        if (not vnd)
                return Status::NullNode;
        
        Status ret = vnd->getdirentn(buf);
        release_node(vnd);
        return ret;
}

} /* namespace kernel::fs::vfs */