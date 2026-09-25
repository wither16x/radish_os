#include <fs/ustar/archive.hpp>
#include <fs/ustar/file.hpp>
#include <fs/ustar/node.hpp>
#include <fs/path.hpp>
#include <lib/conversion.hpp>

namespace Kiwi::Fs::Ustar
{
        void parseArchive(Lib::u8 *archive)
        {
                Lib::u8 *archive_p = archive;

                while (true) {
                        FileHeader *hdr = reinterpret_cast<FileHeader *>(archive_p);
                        if (hdr->name[0] == '\0')
                                break;

                        Lib::usize bytes = Lib::stringToInt<Lib::usize>(hdr->size, Lib::Base::Octal);
                        Lib::String<> path = hdr->name;

                        Lib::Vector<Lib::String<>> parts = parsePath(path);

                        if (parts.isEmpty()) {
                                Lib::usize blocks = (bytes + BLOCK_SIZE - 1) / BLOCK_SIZE;
                                archive_p += BLOCK_SIZE + blocks * BLOCK_SIZE;
                                continue;
                        }

                        Lib::String<> name = parts[parts.length() - 1];

                        Node *parent = root;
                        for (Lib::usize i = 0; i < parts.length() - 1; i++) {
                                Node *next = findDir(parent, parts[i]);
                                if (not next)
                                        next = createDir(parent, parts[i]);
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
                                        nd->dir_data = new Dir;
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
} // namespace Kiwi::Fs::Ustar