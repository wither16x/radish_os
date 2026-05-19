#pragma once

#include <lib/typing.hpp>

namespace kernel::boot::limine {

// Limine base revision
struct Revision {
        lib::u64 magic0;
        lib::u64 magic1;
        lib::u64 rev;

        bool is_supported(this volatile Revision& self);
};

// Limine requests start marker
struct StartMarker {
        lib::u64 magic0;
        lib::u64 magic1;
        lib::u64 magic2;
        lib::u64 magic3;
};

// Limine requests end marker
struct EndMarker {
        lib::u64 magic0;
        lib::u64 magic1;
};

Revision base_revision(lib::u64 revision);
StartMarker requests_start_marker();
EndMarker requests_end_marker();

} /* namespace kernel::boot::limine */