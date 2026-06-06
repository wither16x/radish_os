#pragma once

#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>

// How does this work?
// ----------------------------------------------------------------------------------
// The VFS is an abstraction provided by the kernel to handle different filesystems
// using a unified API.
// The RadishOS VFS is based on "drives", which are identified by a letter from the
// latin alphabet, so a path looks like D:/path/to/something.
// Each file or directory is represented by a virtual node, which contains data about
// the "real" node of the file/dir in the "real" filesystem and a pointer to operations
// that can be executed on the node.

namespace kernel::fs::vfs {

// Operations on a virtual node
struct VNodeOps {
        int (*create_file)(void *fs_data, const lib::String &name);
        int (*create_dir)(void *fs_data, const lib::String &name);
        int (*remove)(void *fs_data);
        int (*write_file)(void *fs_data, const char *buf);
        int (*read_file)(void *fs_data, char *buf, lib::usize n);
        int (*readdir)(void *fs_data, lib::Vector<struct DirEntry> &entries);
        void *(*lookup)(void *fs_data, const lib::String &name);
};

// Virtual node
struct VNode {
        VNodeOps *ops;
        void *fs_data;          // filesystem specific node
};

// Representation of a directory entry
struct DirEntry {
        lib::String name;
        bool is_dir;
};

// One per drive
struct FileSystem {
        virtual ~FileSystem()           = default;

        virtual VNode *get_root()       = 0;
        virtual void unmount()          = 0;
};

struct Drive {
        char id;
        FileSystem *fs;
        VNode *root;
};

// VFS functions
int mount(char id, FileSystem *fs);
int unmount(char id);

int chdrive(char id);

VNode *lookup(const lib::String &path);
int create_file(const lib::String &path);
int create_dir(const lib::String &path);
int remove(const lib::String &path);
int write_file(const lib::String &path, const char *buf);
int read_file(const lib::String &path, char *buf, lib::usize n);
int readdir(const lib::String &path, lib::Vector<DirEntry> &entries);

} /* namespace kernel::fs::vfs */