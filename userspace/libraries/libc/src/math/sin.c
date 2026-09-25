#include "__utils/decls.h"
#include "math.h"

BEGIN_DECLS

/* ------------------------------------------------------------------------------------------------- */
double sin(double arg)
{
        int i = 1;
        double curr = arg;
        double acc = 1;
        double fact = 1;
        double power = arg;

        while (fabs(acc) > 0.00000001 && i < 100) {
                fact *= ((2 * i) * (2 * i + 1));
                power *= -1 * arg * arg;
                acc = power / fact;
                curr += acc;
                ++i;
        }

        return curr;
}
/* ------------------------------------------------------------------------------------------------- */

END_DECLS