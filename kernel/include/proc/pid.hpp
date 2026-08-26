#pragma once

#include <lib/typing.hpp>

namespace Kiwi::Proc
{
        /// Prefer using this type instead of `usize` so that the reader understands directly
        /// that the data is a PID and not a random unsigned integer.
        using pid_t = Lib::usize;

        pid_t allocatePid();
} // namespace Kiwi::Proc