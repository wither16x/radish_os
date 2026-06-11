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
// Each file or directory is represented by a virtual node, which is a base for all
// "real" nodes.

namespace kernel::fs::vfs {

// Virtual node, which represents an entry in the VFS tree.
// "Real" nodes are the nodes from the "real" filesystems.
struct VNode {
        bool owned;

        virtual ~VNode() = default;

        virtual int create_file(const lib::String &name);
        virtual int create_dir(const lib::String &name);
        virtual int remove();
        virtual int write_file(const char *buf, lib::usize n);
        virtual int read_file(char *buf, lib::usize n);
        virtual int readdir(lib::Vector<struct DirEntry> &entries);
        virtual void *lookup(const lib::String &name);
        virtual int get_file_size(lib::usize *buf);
};

// Representation of a directory entry.
// Used in readdir().
struct DirEntry {
        lib::String name;
        bool is_dir;
};

// One per drive.
// Used to execute an operation on the whole filesystem and not
// on a single node.
struct FileSystem {
        virtual ~FileSystem()           = default;

        virtual VNode *get_root()       = 0;
        virtual void unmount()          = 0;
};

// Each drive has one filesystem
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
int write_file(const lib::String &path, const char *buf, lib::usize n);
int read_file(const lib::String &path, char *buf, lib::usize n);
int readdir(const lib::String &path, lib::Vector<DirEntry> &entries);
lib::usize get_file_size(const lib::String &path);

} /* namespace kernel::fs::vfs */