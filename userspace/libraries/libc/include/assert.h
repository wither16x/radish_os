#pragma once

#include "__utils/decls.h"

BEGIN_DECLS

#ifdef NDEBUG
        #define assert(expr) ((void)0)
#else
        void __assert_fail(const char *expr, const char *file, unsigned int line, const char *func);
        #define assert(expr) \
                ((expr) ? (void)0 : __assert_fail(#expr, __FILE__, __LINE__, __func__))
#endif

END_DECLS