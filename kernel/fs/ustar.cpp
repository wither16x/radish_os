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
using kernel::lib::strcpy;

namespace kernel::fs::ustar {

namespace {

static constexpr int BlockSize = 512;

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
// A single header is 512 bytes wide (only 500 bytes are actually used).
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

struct Node {
        Node *parent;

        FileHeader *hdr;
        String name;

        struct File *file_data;
        struct Dir *dir_data;
};

struct File {
        char *data;
};

struct Dir {
        Vector<Node *> nodes;
};

Node *root = nullptr;

// Unsupported operations (cast parameters to `void` to avoid compiler warnings)
// -------------------------------------------------------------------------------
int create_file(void *fs_data, const String &name)
{
        static_cast<void>(fs_data);
        static_cast<void>(name);
        return 0;
}

int create_dir(void *fs_data, const String &name)
{
        static_cast<void>(fs_data);
        static_cast<void>(name);
        return 0;
}

int remove(void *fs_data)
{
        static_cast<void>(fs_data);
        return 0;
}

int write_file(void *fs_data, const char *buf)
{
        static_cast<void>(fs_data);
        static_cast<void>(buf);
        return 0;
}
// -------------------------------------------------------------------------------

int read_file(void *fs_data, char *buf, usize n)
{
        Node *f = static_cast<Node *>(fs_data);
        if (f->hdr->type == static_cast<char>(NodeType::Directory))
                return -1;      // cannot read directories like that
        if (!f->file_data || !f->file_data->data)
                return -2;      // file has no valid data
        if (atoi(f->hdr->size, 8) >= n)
                return -3;      // buffer too small

        strcpy(f->file_data->data, buf);

        return 0;
}

int readdir(void *fs_data, Vector<vfs::DirEntry> &entries)
{
        Node *dir = static_cast<Node *>(fs_data);
        if (!dir->dir_data)
                return -1;              // not a directory

        for (usize i = 0; i < dir->dir_data->nodes.size(); i++) {
                Node *child = dir->dir_data->nodes[i];
                vfs::DirEntry entry;

                entry.name = child->name;
                entry.is_dir = child->hdr->type == static_cast<char>(NodeType::Directory);

                entries.push_back(entry);
        }

        return 0;
}

void *lookup(void *fs_data, const String &name)
{
        Node *dir = static_cast<Node *>(fs_data);
        if (!dir->dir_data)
                return nullptr;

        for (usize i = 0; i < dir->dir_data->nodes.size(); i++) {
                if (dir->dir_data->nodes[i]->name == name)
                        return dir->dir_data->nodes[i];
        }

        return nullptr;
}

Node *find_dir(Node *parent, const String &name)
{
        Node *existing = static_cast<Node *>(lookup(parent, name));
        if (existing && existing->dir_data)
                return existing;

        return nullptr;
}

Node *__create_dir(Node *parent, const String &name)
{
        Node *dir       = new Node;
        dir->parent     = parent;
        dir->hdr        = nullptr;
        dir->name       = name;
        dir->file_data  = nullptr;
        dir->dir_data   = new Dir;

        parent->dir_data->nodes.push_back(dir);

        return dir;
}

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
                        usize blocks = (bytes + BlockSize - 1) / BlockSize;
                        archive_p += BlockSize + blocks * BlockSize;
                        continue;
                }

                String name = parts[parts.size() - 1];

                Node *parent = root;
                for (usize i = 0; i < parts.size() - 1; i++) {
                        parent = find_dir(parent, parts[i]);
                        if (!parent)
                                parent = __create_dir(parent, parts[i]);
                        if (!parent) // failed to create the directory
                                break;
                }

                if (parent) {
                        Node *nd        = new Node;
                        nd->parent      = parent;
                        nd->hdr         = hdr;
                        nd->name        = name;

                        if (hdr->type == static_cast<char>(NodeType::Directory)) {
                                nd->file_data = nullptr;
                                nd->dir_data = new Dir;
                        } else {
                                nd->file_data = new File;
                                nd->file_data->data = reinterpret_cast<char *>(archive_p + BlockSize);
                                nd->dir_data = nullptr;
                        }

                        parent->dir_data->nodes.push_back(nd);
                }

                usize blocks = (bytes + BlockSize - 1) / BlockSize;
                archive_p += BlockSize + blocks * BlockSize;
        }
}

vfs::VNodeOps ustar_ops = {
        .create_file            = create_file,
        .create_dir             = create_dir,
        .remove                 = remove,
        .write_file             = write_file,
        .read_file              = read_file,
        .readdir                = readdir,
        .lookup                 = lookup
};

} /* anonymous namespace */

USTAR::USTAR(void *archive)
{
        this->archive = static_cast<u8 *>(archive);
}

vfs::VNode *USTAR::get_root()
{
        if (!root) {
                root            = new Node;
                root->parent    = nullptr;
                root->hdr       = nullptr;
                root->name      = "/";
                root->file_data = nullptr;
                root->dir_data  = new Dir;

                parse_archive(this->archive);
        }

        vfs::VNode *vnd = new vfs::VNode;
        vnd->ops        = &ustar_ops;
        vnd->fs_data    = root;

        return vnd;
}

void USTAR::unmount()
{
        remove(root);
        root = nullptr;
}

} /* namespace kernel::fs::ustar */