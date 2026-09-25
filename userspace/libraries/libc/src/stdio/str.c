#include "__utils/decls.h"
#include "stdio.h"
#include "string.h"
#include "unistd.h"

BEGIN_DECLS

/* ------------------------------------------------------------------------------------------------- */
int puts(const char *s)
{
        const char *newline = "\n";

        write(fileno(stdout), (const void *)s, strlen(s));
        write(fileno(stdout), (const void *)newline, strlen(newline));

        return 0;
}
/* ------------------------------------------------------------------------------------------------- */

END_DECLS