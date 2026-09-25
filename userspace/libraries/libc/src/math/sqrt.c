#include "__utils/decls.h"
#include "math.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

BEGIN_DECLS

/* ------------------------------------------------------------------------------------------------- */
double sqrt(double arg)
{
        if (arg <= 0)
                return 0;

        double low = MIN(1, arg);
        double high = MAX(1, arg);
        double mid;

        while (100 * low * low < arg)
                low *= 10;
        while (0.01 * high * high > arg)
                high *= 0.1;

        for (int i = 0; i < 100; i++) {
                mid = (low + high) / 2;
                if (mid * mid == arg)
                        return mid;

                if (mid * mid > arg)
                        high = mid;
                else
                        low = mid;
        }

        return mid;
}
/* ------------------------------------------------------------------------------------------------- */

END_DECLS