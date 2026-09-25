#include "__utils/decls.h"
#include "math.h"

BEGIN_DECLS

/* ------------------------------------------------------------------------------------------------- */
double cos(double arg)
{
        arg = fmod(arg, 2 * PI);
        if (arg < 0)
                arg += 2 * PI;
        char sign = 1;
        if (arg > PI) {
                arg -= PI;
                sign = -1;
        }
        return sign * (1 - ((arg * arg) / (2)) + ((arg * arg * arg * arg) / (24)) - ((arg * arg * arg * arg * arg * arg) / (720)) + ((arg * arg * arg * arg * arg * arg * arg * arg) / (40320)) - ((arg * arg * arg * arg * arg * arg * arg * arg * arg * arg) / (3628800)) + ((arg * arg * arg * arg * arg * arg * arg * arg * arg * arg * arg * arg) / (479001600)));
}
/* ------------------------------------------------------------------------------------------------- */

END_DECLS