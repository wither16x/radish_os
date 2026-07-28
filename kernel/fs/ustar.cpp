#include <fs/path.hpp>
#include <fs/ustar.hpp>
#include <fs/vfs.hpp>
#include <lib/conversion.hpp>
#include <lib/memory.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>

using kernel::lib::u8, kernel::lib::usize;
using kernel::lib::atoi;
using kernel::lib::String;
using kernel::lib::Vector;
using kernel::lib::memcpy;

namespace kernel::fs::ustar {

namespace {

constexpr int BLOCK_SIZE = 512;

/// Enumeration of the USTAR file types.
enum class NodeType : char {
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
struct [[gnu::packed]] FileHeader {
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


class USTARFile : public vfs::File {
public:
        char *data;

        vfs::Status read(void *buf, usize size) override;
};

class USTARDir {
public:
        Vector<class Node *> nodes;
};

class Node : public vfs::VNode {
public:
        Node *parent;

        FileHeader *hdr;
        String name;

        class USTARFile *file_data;
        class USTARDir *dir_data;

        vfs::Status readdir(vfs::DirEntry *entry, usize index) override;
        vfs::VNode *lookup(const String &name) override;
        vfs::Status getfilesz(usize *buf) override;
        vfs::Status getdirentn(usize *buf) override;
        vfs::File *open() override;
};

Node *root = nullptr;

// --------------------------------------------------
Node *find_dir(Node *parent, const String &name)
{
        Node *existing = static_cast<Node *>(parent->lookup(name));
        if (existing and existing->dir_data)
                return existing;

        return nullptr;
}
// --------------------------------------------------

// --------------------------------------------------
Node *__create_dir(Node *parent, const String &name)
{
        Node *dir       = new Node;
        dir->parent     = parent;
        dir->hdr        = nullptr;
        dir->name       = name;
        dir->file_data  = nullptr;
        dir->dir_data   = new USTARDir;
        dir->ref_count  = 1;

        parent->dir_data->nodes.push_back(dir);

        return dir;
}
// --------------------------------------------------

// --------------------------------------------------
void parse_archive(u8 *archive)
{
        u8 *archive_p = archive;

        while (true) {
                FileHeader *hdr = reinterpret_cast<FileHeader *>(archive_p);
                if (hdr->name[0] == '\0')
                        break;

                usize bytes = atoi(hdr->size, 8);
                String path = hdr->name;

                Vector<String> parts = parse_path(path);

                if (parts.size() == 0) {
                        usize blocks = (bytes + BLOCK_SIZE - 1) / BLOCK_SIZE;
                        archive_p += BLOCK_SIZE + blocks * BLOCK_SIZE;
                        continue;
                }

                String name = parts[parts.size() - 1];

                Node *parent = root;
                for (usize i = 0; i < parts.size() - 1; i++) {
                        Node *next = find_dir(parent, parts[i]);
                        if (!next)
                                next = __create_dir(parent, parts[i]);
                        parent = next;
                        if (!parent) // failed to create the directory
                                break;
                }

                if (parent) {
                        Node *nd        = new Node;
                        nd->parent      = parent;
                        nd->hdr         = hdr;
                        nd->name        = name;
                        nd->ref_count   = 1;

                        if (hdr->type == static_cast<char>(NodeType::Directory)) {
                                nd->dir_data = new USTARDir;
                                nd->file_data = nullptr;
                        } else {
                                nd->file_data = new USTARFile;
                                nd->file_data->data = reinterpret_cast<char *>(archive_p + BLOCK_SIZE);
                                nd->file_data->size = bytes;
                                nd->dir_data = nullptr;
                        }

                        parent->dir_data->nodes.push_back(nd);
                }

                usize blocks = (bytes + BLOCK_SIZE - 1) / BLOCK_SIZE;
                archive_p += BLOCK_SIZE + blocks * BLOCK_SIZE;
        }
}
// --------------------------------------------------

} /* anonymous namespace */

// --------------------------------------------------
vfs::Status USTARFile::read(void *buf, usize n)
{
        if (not this->data)
                return vfs::Status::NullData;
        if (n < this->size)
                return vfs::Status::OutOfBounds;

        memcpy(buf, this->data, this->size);

        return vfs::Status::Success;
}
// --------------------------------------------------

// --------------------------------------------------
vfs::Status Node::readdir(vfs::DirEntry *entry, usize n)
{
        if (not this->dir_data)
                return vfs::Status::NotADirectory;
        if (n >= this->dir_data->nodes.size())
                return vfs::Status::OutOfBounds;

        Node *nd        = this->dir_data->nodes[n];
        entry->name     = nd->name;
        entry->type     = nd->hdr && nd->hdr->type == static_cast<char>(NodeType::Directory)
                                ? vfs::DirEntryType::Dir
                                : vfs::DirEntryType::File
                        ;

        return vfs::Status::Success;
}
// --------------------------------------------------

// --------------------------------------------------
vfs::VNode *Node::lookup(const String &name)
{
        if (not this->dir_data)
                return nullptr;

        // because root has no parent, the loop below would not work
        if (name == "/")
                return root;

        for (usize i = 0; i < this->dir_data->nodes.size(); i++) {
                if (this->dir_data->nodes[i]->name == name)
                        return this->dir_data->nodes[i];
        }

        return nullptr;
}
// --------------------------------------------------

// --------------------------------------------------
vfs::Status Node::getfilesz(usize *buf)
{
        if (this->dir_data)
                return vfs::Status::IsADirectory;

        memcpy(buf, &this->file_data->size, sizeof(*buf));
        
        return vfs::Status::Success;
}
// --------------------------------------------------

// --------------------------------------------------
vfs::Status Node::getdirentn(usize *buf)
{
        if (not this->dir_data)
                return vfs::Status::NotADirectory;

        usize count = this->dir_data->nodes.size();
        memcpy(buf, &count, sizeof(*buf));

        return vfs::Status::Success;
}
// --------------------------------------------------

vfs::File *Node::open()
{
        if (this->dir_data)
                return nullptr; // is a directory

        return this->file_data;
}

// --------------------------------------------------
USTAR::USTAR(void *archive)
{
        this->archive = static_cast<u8 *>(archive);
}
// --------------------------------------------------

// --------------------------------------------------
vfs::VNode *USTAR::get_root()
{
        if (!root) {
                root            = new Node;
                root->parent    = nullptr;
                root->name      = "/";
                root->file_data = nullptr;
                root->dir_data  = new USTARDir;
                root->ref_count = 1;

                parse_archive(this->archive);
        }

        return root;
}
// --------------------------------------------------

// --------------------------------------------------
vfs::Status USTAR::unmount()
{
        root->rm();
        root = nullptr;

        return vfs::Status::Success;
}
// --------------------------------------------------

} /* namespace kernel::fs::ustar */