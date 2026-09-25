#pragma once

#include <lib/typing.hpp>
#include <fs/vfs/file.hpp>
#include <fs/ustar/decls.hpp>

namespace Kiwi::Fs::Ustar
{
        struct [[gnu::packed]] FileHeader
        {
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

        struct Storage
        {
                char *data = nullptr;
                Lib::usize size = 0;
        };

        struct File : Vfs::File
        {
                Node *owner;
                
                Vfs::Status read(void *buf, Lib::usize size) override;
                Vfs::Status close() override;
        };
} // namespace Kiwi::Fs::Ustar