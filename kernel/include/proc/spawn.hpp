#pragma once

#include <lib/string.hpp>

namespace kernel::proc {

/// Execute a program as a user process.
int spawn(const lib::String &path);

} /* namespace kernel::proc */