#pragma once

#include <fs/vfs/filesystem.hpp>
#include <fs/vfs/node.hpp>

namespace Kiwi::Fs::Ustar
{
        class USTAR : public Vfs::FileSystem
        {
                Lib::u8 *archive;

        public:
                USTAR(void *archive);

                ~USTAR()        = default;

                Vfs::Node *getRoot() override;
                Lib::Result<bool, Vfs::Status> unmount() override;
        };
} // namespace Kiwi::Fs::Ustar