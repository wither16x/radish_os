#pragma once

#include <fs/vfs.hpp>

namespace kernel::fs::tmpfs {

struct TMPFS : public vfs::FileSystem {
        ~TMPFS()           = default;

        vfs::VNode *get_root();
        void unmount();
};

} /* namespace kernel::fs::tmpfs */