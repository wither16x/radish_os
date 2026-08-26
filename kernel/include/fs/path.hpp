#pragma once

#include <lib/string.hpp>
#include <lib/vector.hpp>

namespace Kiwi::Fs
{
        /// Split a path and return all of its part into a vector.
        Lib::Vector<Lib::String> parsePath(const Lib::String &path);
} // namespace Kiwi::Fs