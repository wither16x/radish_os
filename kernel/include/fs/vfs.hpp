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

/// Virtual node, which represents an entry in the VFS tree.
/// "Real" nodes are the nodes from the "real" filesystems.
struct VNode {
        bool owned;

        virtual ~VNode() = default;

        /// Create a new file.
        virtual int touch(const lib::String &name);
        /// Create a new directory.
        virtual int mkdir(const lib::String &name);
        /// Remove a file.
        virtual int remove();
        /// Write bytes to a file.
        virtual int write(const void *buf, lib::usize n);
        /// Read bytes from a file.
        virtual int read(void *buf, lib::usize n);
        /// `n` represents the 0-based index of the entry in the directory
        virtual int readdir(struct DirEntry *entry, lib::usize n);
        /// Look for a file.
        virtual void *lookup(const lib::String &name);
        /// Get the size of a file.
        virtual int getfilesz(lib::usize *buf);
        /// Get the number of entries in a directory.
        virtual int getdirentn(lib::usize *buf);
};

/// Representation of a single directory entry.
struct DirEntry {
        lib::String name;
        bool is_dir;
};

/// One per drive.
/// Used to execute an operation on the whole filesystem and not
/// on a single node.
struct FileSystem {
        virtual ~FileSystem()           = default;

        virtual VNode *get_root()       = 0;
        virtual void unmount()          = 0;
};

/// Each drive has one filesystem.
struct Drive {
        char id;
        FileSystem *fs;
        VNode *root;
};

/// Mount a filesystem to a drive,
int mount(char id, FileSystem *fs);
/// Unmount a drive.
int unmount(char id);
/// Set current drive.
int chdrive(char id);
/// Get current drive.
char getdrive();

VNode *lookup(const lib::String &path);
int touch(const lib::String &path);
int mkdir(const lib::String &path);
int remove(const lib::String &path);
int write(const lib::String &path, const void *buf, lib::usize n);
int read(const lib::String &path, void *buf, lib::usize n);
int readdir(const lib::String &path, DirEntry *entry, lib::usize n);
int getfilesz(const lib::String &path, lib::usize *buf);
int getdirentn(const lib::String &path, lib::usize *buf);

} /* namespace kernel::fs::vfs */