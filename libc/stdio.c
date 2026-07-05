#include <stdio.h>

void print(const char *s)
{
        __asm__ volatile (
                "mov $0, %%rax\n"
                "int $0x80"
                ::
                "b"(s)
        );
}