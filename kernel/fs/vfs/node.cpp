#include <fs/vfs/node.hpp>
#include <fs/vfs/drive.hpp>
#include <fs/path.hpp>

namespace Kiwi::Fs::Vfs
{
        Node *lookupNode(const Lib::String<> &path)
        {
                if (not canPathHandleDrive(path))
                        return nullptr;

                drive_id_t drive = path[0];
                Lib::String<> rel = path.subString(2);

                Drive &drv = getDriveById(drive);

                Lib::Vector<Lib::String<>> parts = parsePath(rel);

                Node *curr_nd = drv.root;
                ++curr_nd->ref_count;

                for (Lib::usize i = 0; i < parts.length(); i++) {
                        void *child = curr_nd->lookup(parts[i]);
                        if (not child) {
                                releaseNode(curr_nd);
                                return nullptr;         // child not found
                        }

                        Node *next = static_cast<Node *>(child);
                        ++next->ref_count;

                        releaseNode(curr_nd);

                        curr_nd = next;
                }

                return curr_nd;
        }

        Status releaseNode(Node *vnd)
        {
                if (not vnd)
                        return Status::NullNode;
                if (vnd->ref_count == 0)
                        return Status::NoRefs;

                --vnd->ref_count;
                if (vnd->ref_count == 0)
                        delete vnd;

                return Status::Success;
        }
} // namespace Kiwi::Fs::Vfs