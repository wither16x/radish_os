#pragma once

#include <fs/vfs/filesystem.hpp>
#include <fs/vfs/node.hpp>

namespace Kiwi::Fs::Vfs
{
        using drive_id_t = unsigned char;

        struct Drive
        {
                drive_id_t id;
                FileSystem *fs;
                Node *root;
        };

        constexpr bool canPathHandleDrive(const Lib::String<> &path)
        {
                return path.length() >= 3;
        }

        Drive &getDriveById(drive_id_t id);
        Node *getNodesFromDriveRoot(const Drive &drv, const Lib::Vector<Lib::String<>> &parts);
        Status mount(drive_id_t drive, FileSystem *fs);
        Status unmount(drive_id_t drive);
} // namespace Kiwi::Fs::Vfs