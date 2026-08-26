#pragma once

#include <proc/pid.hpp>

namespace Kiwi::Proc
{
        /// Create a copy of the current process.
        pid_t fork();
} // namespace Kiwi::Proc