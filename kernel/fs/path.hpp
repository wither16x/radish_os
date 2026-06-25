#pragma once

#include <lib/string.hpp>
#include <lib/vector.hpp>

namespace kernel::fs {

/// Split a path and return all of its part into a vector.
lib::Vector<lib::String> parse_path(const lib::String &path);

} /* namespace kernel::fs */