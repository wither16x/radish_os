#pragma once

namespace kernel::lib {

/// Return the smallest value between `a` and `b`.
inline int min(int a, int b)
{
        if (a < b)
                return a;
        else
                return b;
}

/// Return the biggest value between `a` and `b`.
inline int max(int a, int b)
{
        if (a > b)
                return a;
        else
                return b;
}

} /* namespace kernel::lib */