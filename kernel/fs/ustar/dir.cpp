#include <fs/ustar/dir.hpp>
#include <fs/ustar/node.hpp>

namespace Kiwi::Fs::Ustar
{
        Node *findDir(Node *parent, const Lib::String<> &name)
        {
                Node *existing = static_cast<Node *>(parent->lookup(name));
                if (existing and existing->dir_data)
                        return existing;

                return nullptr;
        }

        Node *createDir(Node *parent, const Lib::String<> &name)
        {
                Node *dir       = new Node;
                dir->parent     = parent;
                dir->hdr        = nullptr;
                dir->name       = name;
                dir->storage    = nullptr;
                dir->dir_data   = new Dir;
                dir->ref_count  = 1;

                parent->dir_data->nodes.pushBack(dir);

                return dir;
        }
} // namespace Kiwi::Fs::Ustar