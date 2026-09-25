#include "__utils/decls.h"
#include "string.h"

BEGIN_DECLS

/* ------------------------------------------------------------------------------------------------- */
char *strcpy(char *dest, const char *src)
{
        if (!dest)
                return NULL;

        char *p = dest;

        while (*src != '\0') {
                *dest = *src;
                dest++;
                src++;
        }

        *dest = '\0';

        return p;
}
/* ------------------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------------------- */
char *strncpy(char *restrict dest, const char *restrict src, size_t count)
{
        if(!dest)
                return NULL;

        char *p = dest;

        while (*src && count--) {
                *dest = *src;
                dest++;
                src++;
        }

        *dest = '\0';

        return p;
}
/* ------------------------------------------------------------------------------------------------- */

END_DECLS