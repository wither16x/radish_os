#pragma once

#include <lib/typing.hpp>

namespace kernel::proc {

using PID = lib::usize;

PID allocate_pid();

} /* namespace kernel::proc */