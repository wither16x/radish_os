#include "__utils/decls.h"
#include "math.h"

BEGIN_DECLS

/* ------------------------------------------------------------------------------------------------- */
double floor(double arg)
{
        long long n = (long long)arg;
        double d = (double)n;

        if (d == arg || arg >= 0)
                return d;
        else
                return d - 1;
}
/* ------------------------------------------------------------------------------------------------- */

END_DECLS