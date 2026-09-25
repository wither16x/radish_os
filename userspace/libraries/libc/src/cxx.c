#include "__utils/decls.h"

BEGIN_DECLS

int __cxa_atexit(void (*func)(void *), void *arg, void *dso_handle)
{
        (void)func;
        (void)arg;
        (void)dso_handle;
        return 0;
}

END_DECLS

void *__dso_hanlde = (void *)&__dso_hanlde;