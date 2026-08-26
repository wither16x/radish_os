#pragma once

#include <fs/vfs.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Fs::Ustar
{
        class USTAR : public Vfs::FileSystem
        {
                Lib::u8 *archive;

        public:
                USTAR(void *archive);

                ~USTAR()        = default;

                Vfs::VNode *getRoot() override;
                Vfs::Status unmount() override;
        };
} // namespace Kiwi::Fs::Ustar