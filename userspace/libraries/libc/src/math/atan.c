#include "__utils/decls.h"
#include "math.h"

BEGIN_DECLS

/* ------------------------------------------------------------------------------------------------- */
double atan(double arg)
{
        double negate = (double)(arg < 0);
        arg = fabs(arg);
        double invert = (double)(arg > 1);
        if (invert)
                arg = 1 / arg;

        double ret = 0.0028662257;
        ret = ret * arg - 0.0161657367;
        ret = ret * arg + 0.0429096138;
        ret = ret * arg - 0.0752896400;
        ret = ret * arg + 0.1065626393;
        ret = ret * arg - 0.1420889944;
        ret = ret * arg + 0.1999355085;
        ret = ret * arg - 0.3333314528;
        ret = ret * arg * arg * arg + arg;

        if (invert)
                ret = (PI / 2) - ret;
        if (negate)
                ret = -ret;

        return ret;
}
/* ------------------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------------------- */
double atan2(double y, double x)
{
        if (x > 0)
                return atan(y / x);
        if (x < 0 && y >= 0)
                return atan(y / x) + PI;
        if (x < 0 && y < 0)
                return atan(y / x) - PI;
        if (x == 0 && y > 0)
                return PI / 2;
        if (x == 0 && y < 0)
                return -PI / 2;
        return 0;
}
/* ------------------------------------------------------------------------------------------------- */

END_DECLS