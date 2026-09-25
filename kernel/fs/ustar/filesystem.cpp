#include <fs/ustar/filesystem.hpp>
#include <fs/ustar/node.hpp>
#include <fs/ustar/archive.hpp>

namespace Kiwi::Fs::Ustar
{
        USTAR::USTAR(void *archive)
        {
                this->archive = static_cast<Lib::u8 *>(archive);
        }

        Vfs::Node *USTAR::getRoot()
        {
                if (not root) {
                        root            = new Node;
                        root->parent    = nullptr;
                        root->name      = "/";
                        root->storage   = nullptr;
                        root->dir_data  = new Dir;
                        root->ref_count = 1;

                        parseArchive(this->archive);
                }

                return root;
        }

        Lib::Result<bool, Vfs::Status> USTAR::unmount()
        {
                root->rm();
                root = nullptr;

                return true;
        }
} // namespace Kiwi::Fs::Ustar