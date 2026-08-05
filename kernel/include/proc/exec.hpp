#pragma once

#include <lib/string.hpp>

namespace kernel::proc {

/// Replace a process by another one.
int exec(const lib::String &path, int argc, char **argv, char **envp);

} /* namespace kernel::proc */