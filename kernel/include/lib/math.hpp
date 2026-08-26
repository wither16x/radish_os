#pragma once

namespace Kiwi::Lib
{
        /// Return the smallest value between `a` and `b`.
        template<typename T>
        inline T min(T a, T b)
        {
                if (a < b)
                        return a;
                else
                        return b;
        }

        /// Return the biggest value between `a` and `b`.
        template<typename T>
        inline T max(T a, T b)
        {
                if (a > b)
                        return a;
                else
                        return b;
        }

        template<typename T>
        inline T clamp(T x, T a, T b)
        {
                return max<T>(a, min<T>(x, b));
        }

        template<typename T>
        T pow(T base, T exp)
        {
                T result = 1;

                while (true) {
                        if (exp & 1)
                                result *= base;

                        exp >>= 1;
                        if (not exp)
                                break;

                        base *= base;
                }

                return result;
        }
} // namespace Kiwi::Lib