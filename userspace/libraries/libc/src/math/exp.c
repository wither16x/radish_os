#include "__utils/decls.h"
#include "math.h"

BEGIN_DECLS

/* ------------------------------------------------------------------------------------------------- */
double exp(double arg)
{
        int neg = 0;
        if (arg < 0) {
                neg = 1;
                arg = -arg;
        }

        int n = (int)(arg / log(2));
        double r = arg - n * log(2);

        double term = 1;
        double sum = 1;
        for (int i = 1; i < 20; i++) {
                term *= r / i;
                sum += term;
        }

        while (n > 0) {
                sum *= 2;
                --n;
        }

        return neg ? 1 / sum : sum;
}
/* ------------------------------------------------------------------------------------------------- */

END_DECLS