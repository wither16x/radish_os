#pragma once

#include <lib/typing.hpp>

namespace Kiwi::Lib
{
        /// Align `n` bytes up to `val`.
        inline usize alignUp(usize val, usize n)
        {
                return (val + n - 1) & ~(n - 1);
        }
} // namespace Kiwi::Lib