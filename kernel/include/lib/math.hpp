#pragma once

namespace kernel::lib {

/// Return the smallest value between `a` and `b`.
inline auto min(auto a, auto b)
{
        if (a < b)
                return a;
        else
                return b;
}

/// Return the biggest value between `a` and `b`.
inline auto max(auto a, auto b)
{
        if (a > b)
                return a;
        else
                return b;
}

inline auto clamp(auto x, auto a, auto b)
{
        return max(a, min(x, b));
}

} /* namespace kernel::lib */