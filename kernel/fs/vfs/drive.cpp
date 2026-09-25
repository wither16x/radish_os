#include <fs/vfs/drive.hpp>
#include <fs/vfs/node.hpp>

namespace Kiwi::Fs::Vfs
{
        /// From A to Z
        constexpr unsigned int MAX_DRIVES = 26;
        Drive drives[MAX_DRIVES];

        Drive &getDriveById(drive_id_t id)
        {
                return drives[id - 'A'];
        }

        Node *getNodesFromDriveRoot(const Drive &drv, const Lib::Vector<Lib::String<>> &parts)
        {
                Node *curr_nd = drv.root;
                for (Lib::usize i = 0; i < parts.length() - 1; i++) {
                        Node *child = curr_nd->lookup(parts[i]);
                        if (not child)
                                return nullptr;

                        curr_nd = static_cast<Node *>(child);
                }

                return curr_nd;
        }

        Status mount(drive_id_t drive, FileSystem *fs)
        {
                Drive &drv = getDriveById(drive);
                if (drv.fs)
                        return Status::FsMounted;

                drv.id = drive;
                drv.fs = fs;
                drv.root = fs->getRoot();
                if (drv.root)
                        ++drv.root->ref_count;

                return Status::Success;
        }

        Status unmount(drive_id_t drive)
        {
                Drive &drv = getDriveById(drive);
                if (not drv.fs)
                        return Status::FsNotMounted;

                drv.fs->unmount();
                delete drv.fs;
                drv.fs = nullptr;
                releaseNode(drv.root);
                drv.root = nullptr;

                return Status::Success;
        }
} // namespace Kiwi::Fs::Vfs