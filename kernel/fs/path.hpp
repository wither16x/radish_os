#pragma once

#include <lib/string.hpp>
#include <lib/vector.hpp>

namespace kernel::fs {

lib::Vector<lib::String> parse_path(const lib::String &path);

} /* namespace kernel::fs */