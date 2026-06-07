#pragma once

#include <fs/vfs.hpp>
#include <lib/typing.hpp>

namespace kernel::fs::ustar {

struct USTAR : public vfs::FileSystem {
public:
        USTAR(void *archive);

        ~USTAR()        = default;

        vfs::VNode *get_root() override;
        void unmount() override;

private:
        lib::u8 *archive;
};

} /* namespace kernel::fs::ustar */