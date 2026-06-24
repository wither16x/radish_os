#pragma once

namespace kernel::lib {

inline int min(int a, int b)
{
        if (a < b)
                return a;
        else
                return b;
}

inline int max(int a, int b)
{
        if (a > b)
                return a;
        else
                return b;
}

} /* namespace kernel::lib */