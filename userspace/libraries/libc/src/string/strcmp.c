#include "__utils/decls.h"
#include "string.h"

BEGIN_DECLS

/* ------------------------------------------------------------------------------------------------- */
int strcmp(const char *lhs, const char *rhs)
{
        while (*lhs && (*lhs == *rhs)) {
                lhs++;
                rhs++;
        }
        return *(unsigned char *)lhs - *(unsigned char *)rhs;
}
/* ------------------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------------------- */
int strncmp(const char *lhs, const char *rhs, size_t count)
{
        while (count &&  *lhs && (*lhs == *rhs)) {
                ++lhs;
                ++rhs;
                --count;
        }

        if (count == 0)
                return 0;

        return *(unsigned char *)lhs - *(unsigned char *)rhs;
}
/* ------------------------------------------------------------------------------------------------- */

END_DECLS