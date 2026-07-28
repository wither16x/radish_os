#pragma once

#include <fs/vfs.hpp>

namespace kernel::fs::tmpfs {

struct TMPFS : public vfs::FileSystem {
        ~TMPFS()           = default;

        vfs::VNode *get_root() override;
        vfs::Status unmount() override;
};

} /* namespace kernel::fs::tmpfs */