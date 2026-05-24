#pragma once

#include <lib/typing.hpp>

namespace kernel::lib {

inline usize align_up(usize val, usize n)
{
        return (val + n - 1) & ~(n - 1);
}

} /* namespace kernel::lib */