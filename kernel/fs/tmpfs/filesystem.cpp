#include <fs/tmpfs/filesystem.hpp>
#include <fs/tmpfs/node.hpp>

namespace Kiwi::Fs::Tmpfs
{
        Vfs::Node *Tmpfs::getRoot()
        {
                if (not root) {
                        root            = new Node;
                        root->parent    = nullptr;
                        root->name      = "/";
                        root->type      = NodeType::Dir;
                        root->dir_data  = new Dir;
                        root->storage   = nullptr;
                        root->file_data = nullptr;
                        root->ref_count = 1;
                }

                return root;
        }

        Lib::Result<bool, Vfs::Status> Tmpfs::unmount()
        {
                if (root) {
                        if (root->dir_data) {
                                for (Lib::usize i = 0; i < root->dir_data->nodes.length(); i++)
                                        removeNode(root->dir_data->nodes[i]);
                                delete root->dir_data;
                        }

                        delete root;
                        root = nullptr;
                }

                return true;
        }
} // namespace Kiwi::Fs::Tmpfs