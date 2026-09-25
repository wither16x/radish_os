#include "__utils/decls.h"
#include "math.h"

BEGIN_DECLS

/* ------------------------------------------------------------------------------------------------- */
double acos(double arg)
{
        double negate = (double)(arg < 0);
        arg = fabs(arg);
        double ret = -0.0187293;
        ret = ret * arg + 0.0742610;
        ret = ret * arg - 0.2121144;
        ret = ret * arg + 1.5707288;
        ret *= sqrt(1.0 - arg);
        ret -= 2 * negate * ret;
        return negate * PI + ret;
}
/* ------------------------------------------------------------------------------------------------- */

END_DECLS