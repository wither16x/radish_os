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

struct Node : public vfs::VNode {
        Node *parent;

        FileHeader *hdr;
        String name;

        struct File *file_data;
        struct Dir *dir_data;

        int read_file(char *buf, usize n) override;
        int readdir(vfs::DirEntry *entry, usize n) override;
        void *lookup(const String &name) override;
        int getfilesz(usize *buf) override;
        int getdirentn(usize *buf) override;
};

struct File {
        char *data;
};

struct Dir {
        Vector<Node *> nodes;
};

Node *root = nullptr;

// --------------------------------------------------
Node *find_dir(Node *parent, const String &name)
{
        Node *existing = static_cast<Node *>(parent->lookup(name));
        if (existing && existing->dir_data)
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
        dir->dir_data   = new Dir;
        dir->owned      = false;

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
                        nd->owned       = false;

                        if (hdr->type == static_cast<char>(NodeType::Directory)) {
                                nd->file_data = nullptr;
                                nd->dir_data = new Dir;
                        } else {
                                nd->file_data = new File;
                                nd->file_data->data = reinterpret_cast<char *>(archive_p + BLOCK_SIZE);
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
int Node::read_file(char *buf, usize n)
{
        if (this->hdr->type == static_cast<char>(NodeType::Directory))
                return -1;      // cannot read directories like that
        if (!this->file_data || !this->file_data->data)
                return -2;      // file has no valid data

        usize filesz = atoi(this->hdr->size, 8);

        if (n < filesz)
                return -3;      // buffer too small

        memcpy(buf, this->file_data->data, filesz);

        return 0;
}
// --------------------------------------------------

// --------------------------------------------------
int Node::readdir(vfs::DirEntry *entry, usize n)
{
        if (!this->dir_data)
                return -1;              // not a directory

        Node *nd = this->dir_data->nodes[n];
        entry->name     = nd->name;
        entry->is_dir   = nd->hdr->type == static_cast<char>(NodeType::Directory); 

        return 0;
}
// --------------------------------------------------

// --------------------------------------------------
void *Node::lookup(const String &name)
{
        if (!this->dir_data)
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
int Node::getfilesz(usize *buf)
{
        if (this->hdr->type == static_cast<char>(NodeType::Directory))
                return -1;      // cannot get the size of a directory like this

        usize filesz = atoi(this->hdr->size, 8);
        memcpy(buf, &filesz, sizeof(*buf));
        
        return 0;
}
// --------------------------------------------------

// --------------------------------------------------
int Node::getdirentn(usize *buf)
{
        if (!this->dir_data)
                return -1;      // not a directory

        usize count = this->dir_data->nodes.size();
        memcpy(buf, &count, sizeof(*buf));

        return 0;
}
// --------------------------------------------------

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
                root->dir_data  = new Dir;
                root->owned     = false;

                parse_archive(this->archive);
        }

        return root;
}
// --------------------------------------------------

// --------------------------------------------------
void USTAR::unmount()
{
        root->remove();
        root = nullptr;
}
// --------------------------------------------------

} /* namespace kernel::fs::ustar */