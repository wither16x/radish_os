#pragma once

#include <lib/string.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Fs::Vfs
{
        using drive_id = unsigned char;

        enum class Status
        {
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

        enum class DirEntryType
        {
                File,
                Dir
        };

        class DirEntry
        {
        public:
                Lib::String name;
                DirEntryType type;
        };

        class VNode
        {
        public:
                virtual ~VNode() = default;

                Lib::usize ref_count = 0;

                virtual class File *open();
                virtual Status mkfile(const Lib::String &name);
                virtual Status mkdir(const Lib::String &name);
                virtual Status rm();
                virtual Status getdirentn(Lib::usize *buf);
                virtual Status readdir(DirEntry *entry, Lib::usize index) = 0;
                virtual Status getfilesz(Lib::usize *buf);
                virtual VNode *lookup(const Lib::String &name);
        };

        class File
        {
        public:
                virtual ~File() = default;

                VNode *vnode;
                Lib::usize size;
                Lib::usize ref_count = 0;

                virtual Status write(const void *buf, Lib::usize size);
                virtual Status read(void *buf, Lib::usize size);
                virtual Status close();
        };

        class FileSystem
        {
        public:
                virtual ~FileSystem() = default;

                virtual VNode *getRoot() = 0;
                virtual Status unmount() = 0;
        };

        class Drive
        {
        public:
                drive_id id;
                FileSystem *fs;
                VNode *root;
        };

        Status mount(drive_id drive, FileSystem *fs);
        Status unmount(drive_id drive);

        VNode *lookupNode(const Lib::String &path);
        Status releaseNode(VNode *vnode);

        File *openFile(const Lib::String &path);
        Status closeFile(File *file);
        Status mkfile(const Lib::String &path);
        Status mkdir(const Lib::String &path);
        Status remove(const Lib::String &path);
        Status write(File *file, const void *buf, Lib::usize size);
        Status read(File *file, void *buf, Lib::usize size);
        Status readdir(const Lib::String &path, DirEntry *entry, Lib::usize index);
        Status getfilesz(const Lib::String &path, Lib::usize *buf);
        Status getdirentn(const Lib::String &path, Lib::usize *buf);
} // namespace Kiwi::Fs::Vfs