#include <fs/devfs/node.hpp>
#include <fs/devfs/filesystem.hpp>
#include <fs/devfs/file.hpp>

namespace Kiwi::Fs::Devfs
{
        Vfs::Node *Devfs::getRoot()
        {
                if (not root) {
                        root            = new Node;
                        root->name      = "/";
                        root->type      = NodeType::Root;
                        root->devtype   = DeviceType::None;
                        root->file_data = nullptr;
                        root->ref_count = 1;
                }

                return root;
        }

        Lib::Result<bool, Vfs::Status> Devfs::unmount()
        {
                for (Lib::usize i = 0; i < devices.length(); i++) {
                        if (devices[i]->file_data)
                                delete devices[i]->file_data;
                        delete devices[i];
                }

                delete root;
                root = nullptr;

                return true;
        }
} // namespace Kiwi::Fs::Devfs