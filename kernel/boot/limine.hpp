#pragma once

#include <lib/typing.hpp>

using namespace kernel::typing;

namespace kernel {

namespace boot {

namespace limine {

// Limine base revision
struct revision {
        u64 magic0;
        u64 magic1;
        u64 rev;

        bool is_supported(this volatile revision& self);
};

// Limine requests start marker
struct start_marker {
        u64 magic0;
        u64 magic1;
        u64 magic2;
        u64 magic3;
};

// Limine requests end marker
struct end_marker {
        u64 magic0;
        u64 magic1;
};

revision base_revision(u64 revision);
start_marker requests_start_marker();
end_marker requests_end_marker();

} /* namespace limine */

} /* namespace boot */

} /* namespace kernel */