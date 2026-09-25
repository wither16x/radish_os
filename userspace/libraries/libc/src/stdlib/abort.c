#include "__utils/decls.h"
#include "stdlib.h"

BEGIN_DECLS

extern void tomato_end();

void abort(void)
{
        tomato_end();
}

END_DECLS