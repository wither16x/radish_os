#include "__utils/decls.h"
#include "stdio.h"
#include "unistd.h"

extern void (*__init_array_start[])(void);
extern void (*__init_array_end[])(void);

static FILE __stdin_file;
static FILE __stdout_file;

FILE *stdin = &__stdin_file;
FILE *stdout = &__stdout_file;
FILE *stderr = &__stdout_file;

BEGIN_DECLS

void tomato_init()
{
        int stdin_fd = open("D:/input", 0);
        int stdout_fd = open("D:/console", 0);

        __stdin_file.fd = stdin_fd;
        __stdout_file.fd = stdout_fd;

        for (void (**fn)(void) = __init_array_start; fn != __init_array_end; ++fn)
                (*fn)();
}

void tomato_end()
{
        exit();
}

END_DECLS