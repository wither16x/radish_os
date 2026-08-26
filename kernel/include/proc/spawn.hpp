#pragma once

#include <lib/string.hpp>

namespace Kiwi::Proc
{
        /// Execute a program as a user process.
        int spawn(const Lib::String &path);
} // namespace Kiwi::Proc