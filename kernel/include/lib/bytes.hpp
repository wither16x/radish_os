#pragma once

#include <lib/typing.hpp>

namespace Kiwi::Lib
{
        /// Align `n` bytes up to `val`.
        template<typename T>
        inline T alignUp(T val, T n)
        {
                if (n == 0)
                        return 0; // n is not zero

                T rem = val % n;
                if (rem == 0)
                        return val;

                T res = val + (n - rem);
                if (res < val)
                        return 0; // overflow

                return res;
        }
} // namespace Kiwi::Lib