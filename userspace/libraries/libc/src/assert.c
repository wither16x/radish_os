#include "assert.h"
#include "__utils/decls.h"
#include "stdio.h"

BEGIN_DECLS

extern void tomato_end();

void __assert_fail(const char *expr, const char *file, unsigned int line, const char *func)
{
        printf("%s failed in %s when calling %s() at line %s\n", expr, file, func, line);
        tomato_end();
}

END_DECLS