#include "__utils/decls.h"
#include "string.h"
#include "stdio.h"

BEGIN_DECLS

char *strerror(int errnum)
{
        /* TODO: add a message table instead of... this... */
        static char buf[32];
        snprintf(buf, sizeof(buf), "error %d", errnum);
        return buf;
}

END_DECLS