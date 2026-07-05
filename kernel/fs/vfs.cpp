#include <fs/path.hpp>
#include <fs/vfs.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>

using kernel::lib::String;
using kernel::lib::usize;
using kernel::lib::Vector;

namespace kernel::fs::vfs {

namespace {

constexpr int MAX_DRIVES = 26;

Drive drives[MAX_DRIVES];
char current_drive_id = 'A';            // default drive

/// Find a drive using its ID.
Drive *get_drive_by_id(char id)
{
        for (int i = 0; i < MAX_DRIVES; i++) {
                if (drives[i].id == id)
                        return &drives[i];
        }

        return nullptr;
}

/// Check if a path is long enough to handle `D:/`.
bool path_can_handle_drive(const String &path)
{
        return path.length() >= 3;
}

} /* anonymous namespace */

// --------------------------------------------------------------------
int VNode::touch(const String &name)
{
        static_cast<void>(name);
        return 0;
}
// --------------------------------------------------

// --------------------------------------------------
int VNode::mkdir(const String &name)
{
        static_cast<void>(name);
        return 0;
}
// --------------------------------------------------

// --------------------------------------------------
int VNode::remove()
{
        return 0;
}
// --------------------------------------------------

// --------------------------------------------------
int VNode::write(const char *buf, usize n)
{
        static_cast<void>(buf);
        static_cast<void>(n);
        return 0;
}
// --------------------------------------------------

// --------------------------------------------------
int VNode::read(char *buf, usize n)
{
        static_cast<void>(buf);
        static_cast<void>(n);
        return 0;
}
// --------------------------------------------------

// --------------------------------------------------
int VNode::readdir(DirEntry *entry, usize n)
{
        static_cast<void>(entry);
        static_cast<void>(n);
        return 0;
}
// --------------------------------------------------

// --------------------------------------------------
void *VNode::lookup(const String &name)
{
        static_cast<void>(name);
        return 0;
}
// --------------------------------------------------

// --------------------------------------------------
int VNode::getfilesz(usize *buf)
{
        static_cast<void>(buf);
        return 0;
}
// --------------------------------------------------

// --------------------------------------------------
int VNode::getdirentn(lib::usize *buf)
{
        static_cast<void>(buf);
        return 0;
}
// --------------------------------------------------

// --------------------------------------------------
int mount(char id, FileSystem *fs)
{
        Drive *drv = &drives[id - 'A'];
        if (drv->fs)
                return -1;              // already mounted

        drv->id = id;
        drv->fs = fs;
        drv->root = fs->get_root();

        return 0;
}
// --------------------------------------------------

// --------------------------------------------------
int unmount(char id)
{
        Drive *drv = &drives[id - 'A'];
        if (!drv->fs)
                return -1;              // not mounted

        drv->fs->unmount();
        delete drv->fs;
        drv->fs = nullptr;
        drv->root = nullptr;

        return 0;
}
// --------------------------------------------------

// --------------------------------------------------
int chdrive(char id)
{
        if (id >= 'A' && id <= 'Z') {
                current_drive_id = id;
                return 0;
        }

        return -1;
}
// --------------------------------------------------

char getdrive()
{
        return current_drive_id;
}

// --------------------------------------------------
VNode *lookup(const lib::String &path)
{
        if (!path_can_handle_drive(path))
                return nullptr;         // path too small for D:/

        char drive_id = path[0];
        String rel = path.sub(2);

        Drive *drv = get_drive_by_id(drive_id);
        if (!drv || !drv->root)
                return nullptr;         // drive not found or drive does not have a root directory

        Vector<String> parts = parse_path(rel);

        VNode *curr_nd = drv->root;
        for (usize i = 0; i < parts.size(); i++) {
                void *child = curr_nd->lookup(parts[i]);
                if (!child)
                        return nullptr;         // child not found

                curr_nd = static_cast<VNode *>(child);
        }

        return curr_nd;
}
// --------------------------------------------------

// --------------------------------------------------
int touch(const lib::String &path)
{
        if (!path_can_handle_drive(path))
                return -1;      // path is too small for D:/

        char drive_id = path[0];
        String rel = path.sub(2);

        Drive *drv = get_drive_by_id(drive_id);
        if (!drv || !drv->root)
                return -2;      // drive not found or does not have a rootdir

        Vector<String> parts = parse_path(rel);

        VNode *curr_nd = drv->root;

        for (usize i = 0; i < parts.size() - 1; i++) {
                void *child = curr_nd->lookup(parts[i]);
                if (!child)
                        return -3;      // child not found

                curr_nd = static_cast<VNode *>(child);
        }

        return curr_nd->touch(parts[parts.size() - 1]);
}
// --------------------------------------------------

// --------------------------------------------------
int mkdir(const lib::String &path)
{
        if (!path_can_handle_drive(path))
                return -1;      // path is too small for D:/

        char drive_id = path[0];
        String rel = path.sub(2);

        Drive *drv = get_drive_by_id(drive_id);
        if (!drv || !drv->root)
                return -2;      // drive not found or does not have a rootdir

        Vector<String> parts = parse_path(rel);

        VNode *curr_nd = drv->root;
        for (usize i = 0; i < parts.size() - 1; i++) {
                void *child = curr_nd->lookup(parts[i]);
                if (!child)
                        return -3;      // child not found

                curr_nd = static_cast<VNode *>(child);
        }

        return curr_nd->mkdir(parts[parts.size() - 1]);
}
// --------------------------------------------------

// --------------------------------------------------
int remove(const lib::String &path)
{
        VNode *vnd = lookup(path);
        if (!vnd)
                return -1;      // file/dir not found

        return vnd->remove();
}
// --------------------------------------------------

// --------------------------------------------------
int write(const String &path, const char *buf, usize n)
{
        VNode *vnd = lookup(path);
        if (!vnd)
                return -1;      // file not found

        return vnd->write(buf, n);
}
// --------------------------------------------------

// --------------------------------------------------
int read(const String &path, char *buf, usize n)
{
        VNode *vnd = lookup(path);
        if (!vnd)
                return -1;      // file not found

        int ret = vnd->read(buf, n);
        if (vnd->owned)
                delete vnd;
        return ret;
}
// --------------------------------------------------

// --------------------------------------------------
int readdir(const String &path, DirEntry *entry, usize n)
{
        VNode *vnd = lookup(path);
        if (!vnd)
                return -1;      // directory not found

        int ret = vnd->readdir(entry, n);

        if (vnd->owned)
                delete vnd;

        return ret;
}
// --------------------------------------------------

// --------------------------------------------------
int getfilesz(const lib::String &path, usize *buf)
{
        VNode *vnd = lookup(path);
        if (!vnd)
                return 0;      // file not found

        int ret = vnd->getfilesz(buf);
        
        if (vnd->owned)
                delete vnd;
        
        return ret;
}
// --------------------------------------------------

// --------------------------------------------------
int getdirentn(const lib::String &path, lib::usize *buf)
{
        VNode *vnd = lookup(path);
        if (!vnd)
                return -1;       // directory not found
        
        int ret = vnd->getdirentn(buf);

        if (vnd->owned)
                delete vnd;

        return ret;
}
// --------------------------------------------------

} /* namespace kernel::fs::vfs */