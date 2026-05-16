#pragma once

#include <lib/typing.hpp>

namespace kernel::boot::limine {

// Limine base revision
struct revision {
        lib::u64 magic0;
        lib::u64 magic1;
        lib::u64 rev;

        bool is_supported(this volatile revision& self);
};

// Limine requests start marker
struct start_marker {
        lib::u64 magic0;
        lib::u64 magic1;
        lib::u64 magic2;
        lib::u64 magic3;
};

// Limine requests end marker
struct end_marker {
        lib::u64 magic0;
        lib::u64 magic1;
};

revision base_revision(lib::u64 revision);
start_marker requests_start_marker();
end_marker requests_end_marker();

} /* namespace kernel::boot::limine */