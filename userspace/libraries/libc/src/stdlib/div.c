#include "__utils/decls.h"
#include "stdlib.h"

#define __DIV_CORE(T, x, y) \
        do { \
                T result; \
                result.quot = x / y; \
                result.rem = x % y; \
                return result; \
        } while (0)

BEGIN_DECLS

div_t div(int x, int y)
{
        __DIV_CORE(div_t, x, y);
}

ldiv_t ldiv(long x, long y)
{
        __DIV_CORE(ldiv_t, x, y);
}

lldiv_t lldiv(long long x, long long y)
{
        __DIV_CORE(lldiv_t, x, y);
}

END_DECLS