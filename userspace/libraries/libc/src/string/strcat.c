#include "__utils/decls.h"
#include "string.h"

BEGIN_DECLS

char *strcat(char *restrict dest, const char *restrict src)
{
        char *ptr = dest + strlen(dest);

        while (*src != '\0')
                *ptr++ = *src++;
        *ptr = '\0';

        return dest;
}

END_DECLS