#pragma once

#include <lib/typing.hpp>

namespace kernel::proc {

/// Prefer using this type instead of `usize` so that the reader understands directly
/// that the data is a PID and not a random unsigned integer.
using PID = lib::usize;

PID allocate_pid();

} /* namespace kernel::proc */