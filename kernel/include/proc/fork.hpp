#pragma once

#include <proc/pid.hpp>

namespace kernel::proc {

/// Create a copy of the current process.
PID fork();

} /* namespace kernel::proc */