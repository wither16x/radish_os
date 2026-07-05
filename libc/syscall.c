#include "syscall.h"
#include "stddef.h"

int write(const char *file, const char *s, size_t n)
{
        __asm__ volatile (
                "int $0x80"
                ::
                "a"(0), "b"(file), "c"(s), "d"(n)
        );

        register int ret __asm__("rax");
        return ret;
}