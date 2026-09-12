#pragma once

#include <lib/typing.hpp>

namespace Kiwi::Lib
{
        /// Align `n` bytes up to `val`.
        inline usize alignUp(usize val, usize n)
        {
                if (n == 0)
                        return 0; // n is not zero

                usize rem = val % n;
                if (rem == 0)
                        return val;

                usize res = val + (n - rem);
                if (res < val)
                        return 0; // overflow

                return res;
        }
} // namespace Kiwi::Lib