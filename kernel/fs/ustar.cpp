#include <fs/path.hpp>
#include <fs/ustar.hpp>
#include <fs/vfs.hpp>
#include <lib/conversion.hpp>
#include <lib/memory.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>

namespace Kiwi::Fs::Ustar
{
        namespace
        {
                constexpr int BLOCK_SIZE = 512;

                /// Enumeration of the USTAR file types.
                enum class NodeType : char
                {
                        NormalFile      = '0',
                        HardLink        = '1',
                        SymbolicLink    = '2',
                        CharSpecialDev  = '3',
                        BlockDev        = '4',
                        Directory       = '5',
                        NamedPipe       = '6'
                };

                // bruh why is everything in octal
                //
                /// A single header is 512 bytes wide (only 500 bytes are actually used).
                struct [[gnu::packed]] FileHeader
                {
                        char name[100];
                        char mode[8];           // octal
                        char oid[8];            // owner id (octal)
                        char gid[8];            // group id (octal)
                        char size[12];          // file size in bytes (octal)
                        char last[12];          // last modification time (octal)
                        char checksum[8];
                        char type;
                        char linked_name[100];  // name of linked file
                        char indicator[6];
                        char version[2];
                        char user[32];
                        char group[32];
                        char devmaj[8];         // device major number
                        char devmin[8];         // device minor number
                        char prefix[155];
                };

                class Storage
                {
                public:
                        char *data = nullptr;
                        Lib::usize size = 0;
                };

                class UstarFile : public Vfs::File
                {
                public:
                        class Node *owner;

                        Vfs::Status read(void *buf, Lib::usize size) override;
                        Vfs::Status close() override;
                };

                class UstarDir
                {
                public:
                        Lib::Vector<class Node *> nodes;
                };

                class Node : public Vfs::VNode
                {
                public:
                        Node *parent;

                        FileHeader *hdr;
                        Lib::String name;

                        Storage *storage;
                        class UstarDir *dir_data;

                        Vfs::Status readdir(Vfs::DirEntry *entry, Lib::usize index) override;
                        Vfs::VNode *lookup(const Lib::String &name) override;
                        Vfs::Status getfilesz(Lib::usize *buf) override;
                        Vfs::Status getdirentn(Lib::usize *buf) override;
                        Vfs::File *open() override;
                };

                Node *root = nullptr;

                Node *findDir(Node *parent, const Lib::String &name)
                {
                        Node *existing = static_cast<Node *>(parent->lookup(name));
                        if (existing and existing->dir_data)
                                return existing;

                        return nullptr;
                }

                Node *__createDir(Node *parent, const Lib::String &name)
                {
                        Node *dir       = new Node;
                        dir->parent     = parent;
                        dir->hdr        = nullptr;
                        dir->name       = name;
                        dir->storage    = nullptr;
                        dir->dir_data   = new UstarDir;
                        dir->ref_count  = 1;

                        parent->dir_data->nodes.pushBack(dir);

                        return dir;
                }

                void parseArchive(Lib::u8 *archive)
                {
                        Lib::u8 *archive_p = archive;

                        while (true) {
                                FileHeader *hdr = reinterpret_cast<FileHeader *>(archive_p);
                                if (hdr->name[0] == '\0')
                                        break;

                                Lib::usize bytes = Lib::atoi(hdr->size, 8, sizeof(hdr->size));
                                Lib::String path = hdr->name;

                                Lib::Vector<Lib::String> parts = parsePath(path);

                                if (parts.empty()) {
                                        Lib::usize blocks = (bytes + BLOCK_SIZE - 1) / BLOCK_SIZE;
                                        archive_p += BLOCK_SIZE + blocks * BLOCK_SIZE;
                                        continue;
                                }

                                Lib::String name = parts[parts.size() - 1];

                                Node *parent = root;
                                for (Lib::usize i = 0; i < parts.size() - 1; i++) {
                                        Node *next = findDir(parent, parts[i]);
                                        if (not next)
                                                next = __createDir(parent, parts[i]);
                                        parent = next;
                                        if (not parent) // failed to create the directory
                                                break;
                                }

                                if (parent) {
                                        Node *nd        = new Node;
                                        nd->parent      = parent;
                                        nd->hdr         = hdr;
                                        nd->name        = name;
                                        nd->ref_count   = 1;

                                        if (hdr->type == static_cast<char>(NodeType::Directory)) {
                                                nd->dir_data = new UstarDir;
                                                nd->storage = nullptr;
                                        } else {
                                                nd->storage = new Storage;
                                                nd->storage->data = reinterpret_cast<char *>(archive_p + BLOCK_SIZE);
                                                nd->storage->size = bytes;
                                                nd->dir_data = nullptr;
                                        }

                                        parent->dir_data->nodes.pushBack(nd);
                                }

                                Lib::usize blocks = (bytes + BLOCK_SIZE - 1) / BLOCK_SIZE;
                                archive_p += BLOCK_SIZE + blocks * BLOCK_SIZE;
                        }
                }

        } /* anonymous namespace */

        Vfs::Status UstarFile::read(void *buf, Lib::usize n)
        {
                if (not this->owner or not this->owner->storage or not this->owner->storage->data)
                        return Vfs::Status::NullData;
                if (n < this->owner->storage->size)
                        return Vfs::Status::OutOfBounds;

                Lib::memcpy(reinterpret_cast<Lib::uptr *>(buf) + this->cursor, this->owner->storage->data, n);

                return Vfs::Status::Success;
        }

        Vfs::Status UstarFile::close()
        {
                return Vfs::Status::Success;
        }

        Vfs::Status Node::readdir(Vfs::DirEntry *entry, Lib::usize n)
        {
                if (not this->dir_data)
                        return Vfs::Status::NotADirectory;
                if (n >= this->dir_data->nodes.size())
                        return Vfs::Status::OutOfBounds;

                Node *nd        = this->dir_data->nodes[n];
                entry->name     = nd->name;
                entry->type     = nd->dir_data ? Vfs::DirEntryType::Dir : Vfs::DirEntryType::File;

                return Vfs::Status::Success;
        }

        Vfs::VNode *Node::lookup(const Lib::String &name)
        {
                if (not this->dir_data)
                        return nullptr;

                // because root has no parent, the loop below would not work
                if (name == "/")
                        return root;

                for (Lib::usize i = 0; i < this->dir_data->nodes.size(); i++) {
                        if (this->dir_data->nodes[i]->name == name)
                                return this->dir_data->nodes[i];
                }

                return nullptr;
        }

        Vfs::Status Node::getfilesz(Lib::usize *buf)
        {
                if (this->dir_data)
                        return Vfs::Status::IsADirectory;
                if (not this->storage)
                        return Vfs::Status::NullData;

                Lib::memcpy(buf, &this->storage->size, sizeof(*buf));
                
                return Vfs::Status::Success;
        }

        Vfs::Status Node::getdirentn(Lib::usize *buf)
        {
                if (not this->dir_data)
                        return Vfs::Status::NotADirectory;

                Lib::usize count = this->dir_data->nodes.size();
                Lib::memcpy(buf, &count, sizeof(*buf));

                return Vfs::Status::Success;
        }

        Vfs::File *Node::open()
        {
                if (this->dir_data)
                        return nullptr; // is a directory

                UstarFile *file = new UstarFile;
                file->owner = this;

                return file;
        }

        USTAR::USTAR(void *archive)
        {
                this->archive = static_cast<Lib::u8 *>(archive);
        }

        Vfs::VNode *USTAR::getRoot()
        {
                if (not root) {
                        root            = new Node;
                        root->parent    = nullptr;
                        root->name      = "/";
                        root->storage   = nullptr;
                        root->dir_data  = new UstarDir;
                        root->ref_count = 1;

                        parseArchive(this->archive);
                }

                return root;
        }

        Vfs::Status USTAR::unmount()
        {
                root->rm();
                root = nullptr;

                return Vfs::Status::Success;
        }
} // namespace Kiwi::Fs::Ustar