#pragma once

#include <lib/string.hpp>
#include <lib/typing.hpp>

namespace kernel::fs::vfs {

using drive_id = unsigned char;

enum class Status : unsigned int {
        Success,
        FsMounted,
        FsNotMounted,
        NullNode,
        NoRefs,
        NullFile,
        PathTooShort,
        NullRoot,
        EmptyPath,
        ChildNotFound,
        IsADirectory,
        NullData,
        OutOfBounds,
        NotADirectory,
        NotImplemented
};

enum class DirEntryType : unsigned int {
        File,
        Dir
};

class DirEntry {
public:
        lib::String name;
        DirEntryType type;
};

class VNode {
public:
        virtual ~VNode() = default;

        lib::usize ref_count = 0;

        virtual class File *open();
        virtual Status mkfile(const lib::String &name);
        virtual Status mkdir(const lib::String &name);
        virtual Status rm();
        virtual Status getdirentn(lib::usize *buf);
        virtual Status readdir(DirEntry *entry, lib::usize index) = 0;
        virtual Status getfilesz(lib::usize *buf);
        virtual VNode *lookup(const lib::String &name);
};

class File {
public:
        virtual ~File() = default;

        VNode *vnode;
        lib::usize size;

        virtual Status write(const void *buf, lib::usize size);
        virtual Status read(void *buf, lib::usize size);
};

class FileSystem {
public:
        virtual ~FileSystem() = default;

        virtual VNode *get_root() = 0;
        virtual Status unmount() = 0;
};

class Drive {
public:
        drive_id id;
        FileSystem *fs;
        VNode *root;
};

Status mount(drive_id drive, FileSystem *fs);
Status unmount(drive_id drive);

VNode *lookup_node(const lib::String &path);
Status release_node(VNode *vnode);

File *open_file(const lib::String &path);
Status close_file(File *file);
Status mkfile(const lib::String &path);
Status mkdir(const lib::String &path);
Status remove(const lib::String &path);
Status write(File *file, const void *buf, lib::usize size);
Status read(File *file, void *buf, lib::usize size);
Status readdir(const lib::String &path, DirEntry *entry, lib::usize index);
Status getfilesz(const lib::String &path, lib::usize *buf);
Status getdirentn(const lib::String &path, lib::usize *buf);

} /* namespace kernel::fs::vfs */