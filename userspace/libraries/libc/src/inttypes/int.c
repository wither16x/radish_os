#include "__utils/decls.h"
#include "inttypes.h"

BEGIN_DECLS

intmax_t imaxabs(intmax_t j)
{
        if (j >= 0)
                return j;
        return -j;
}

imaxdiv_t imaxdiv(intmax_t number, intmax_t demon)
{
        imaxdiv_t result;
        result.quot = number / demon;
        result.rem = number % demon;
        return result;
}

END_DECLS