#include "__utils/decls.h"
#include "stdlib.h"

BEGIN_DECLS

/*
        There is no shell environment for now.
*/
char *getenv(const char *name)
{
        (void)name;
        return NULL;
}

END_DECLS