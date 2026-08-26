#pragma once

#include <lib/typing.hpp>

namespace Kiwi::Boot::Limine
{
        /// Limine base revision representation using a structure
        /// instead of a raw array, which is what `limine.h` does.
        struct Revision
        {
                Lib::u64 magic0;
                Lib::u64 magic1;
                Lib::u64 rev;

                bool isSupported(this volatile Revision &self);
        };

        /// Limine requests start marker representation using a
        /// structure instead of a raw array, which is what `limine.h`
        /// does.
        struct StartMarker
        {
                Lib::u64 magic0;
                Lib::u64 magic1;
                Lib::u64 magic2;
                Lib::u64 magic3;
        };

        /// Limine requests end marker representation using a
        /// structure instead of a raw array, which is what 
        // `limine.h` does.
        struct EndMarker
        {
                Lib::u64 magic0;
                Lib::u64 magic1;
        };

        /// Wrapper around the `LIMINE_BASE_REVISION()` macro
        /// provided by `limine.h`.
        Revision baseRevision(Lib::u64 revision);
        /// Wrapper around the `LININE_REQUESTS_START_NARKER` macro
        /// provided by `limine.h`.
        StartMarker requestsStartMarker();
        /// Wrapper around the `LININE_REQUESTS_END_NARKER` macro
        /// provided by `limine.h`.
        EndMarker requestsEndMarker();
        volatile Revision &getBaseRevision();
} // namespace Kiwi::Boot::Limine