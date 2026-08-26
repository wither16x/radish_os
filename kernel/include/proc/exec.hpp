#pragma once

#include <lib/string.hpp>

namespace Kiwi::Proc {
        /// Replace a process by another one.
        int exec(const Lib::String &path, int argc, char **argv, char **envp);
} // namespace Kiwi::Proc