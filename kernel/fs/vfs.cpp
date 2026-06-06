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

constexpr int MaxDrives = 26;

Drive drives[MaxDrives];
char current_drive_id = 'A';            // default drive

Drive *get_drive_by_id(char id)
{
        for (int i = 0; i < MaxDrives; i++) {
                if (drives[i].id == id)
                        return &drives[i];
        }

        return nullptr;
}

bool path_can_handle_drive(const String &path)
{
        return path.length() >= 3;
}

} /* anonymous namespace */

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

int chdrive(char id)
{
        if (id >= 'A' && id <= 'Z') {
                current_drive_id = id;
                return 0;
        }

        return -1;
}

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
                void *child = curr_nd->ops->lookup(curr_nd->fs_data, parts[i]);
                if (!child)
                        return nullptr;         // child not found

                VNode *next_nd          = new VNode;
                next_nd->ops            = curr_nd->ops;
                next_nd->fs_data        = child;

                if (curr_nd != drv->root)
                        delete curr_nd;

                curr_nd = next_nd;
        }

        return curr_nd;
}

int create_file(const lib::String &path)
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
                void *child = curr_nd->ops->lookup(curr_nd->fs_data, parts[i]);
                if (!child)
                        return -3;      // child not found

                VNode *next_nd          = new VNode;
                next_nd->ops            = curr_nd->ops;
                next_nd->fs_data        = child;

                if (curr_nd != drv->root)
                        delete curr_nd;

                curr_nd = next_nd;
        }

        int ret = curr_nd->ops->create_file(curr_nd->fs_data, parts[parts.size() - 1]);
        
        if (curr_nd != drv->root)
                delete curr_nd;

        return ret;
}

int create_dir(const lib::String &path)
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
                void *child = curr_nd->ops->lookup(curr_nd->fs_data, parts[i]);
                if (!child)
                        return -3;      // child not found

                VNode *next_nd          = new VNode;
                next_nd->ops            = curr_nd->ops;
                next_nd->fs_data        = child;

                if (curr_nd != drv->root)
                        delete curr_nd;

                curr_nd = next_nd;
        }

        int ret = curr_nd->ops->create_dir(curr_nd->fs_data, parts[parts.size() - 1]);
        
        if (curr_nd != drv->root)
                delete curr_nd;

        return ret;
}

int write_file(const String &path, const char *buf)
{
        VNode *vnd = lookup(path);
        if (!vnd)
                return -1;      // file not found

        return vnd->ops->write_file(vnd->fs_data, buf);
}

int read_file(const String &path, char *buf, usize n)
{
        VNode *vnd = lookup(path);
        if (!vnd)
                return -1;      // file not found

        return vnd->ops->read_file(vnd->fs_data, buf, n);
}

int readdir(const String &path, Vector<DirEntry> &entries)
{
        VNode *vnd = lookup(path);
        if (!vnd)
                return -1;      // directory not found

        return vnd->ops->readdir(vnd->fs_data, entries);
}

} /* namespace kernel::fs::vfs */