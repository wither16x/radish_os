#pragma once

#include <lib/string.hpp>
#include <proc/process.hpp>

namespace kernel::proc {

/// Load an executable and create a process which uses it as its code.
Process *load_as_proc(const lib::String &path);
/// Execute a program as a user process.
int spawn(const lib::String &path);

} /* namespace kernel::proc */