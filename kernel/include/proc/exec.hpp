#pragma once

#include <lib/string.hpp>
#include <proc/process.hpp>

namespace kernel::proc {

/// Execute a program as a user process.
int spawn(const lib::String &path);
/// Replace a process by another one.
int exec(const lib::String &path);

} /* namespace kernel::proc */