#pragma once

#include <proc/pid.hpp>

namespace kernel::proc {

int kill(PID pid);

} /* namespace kernel::proc */