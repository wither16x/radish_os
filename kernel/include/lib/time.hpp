#pragma once

#include <lib/typing.hpp>

namespace Kiwi::Lib
{
        /// Sleep `ms` miliseconds.
        void sleep(u64 ms);
        /// Return the current time in miliseconds since
        /// timer initialization.
        u64 getCurrentTime();
} // namespace Kiwi::Lib