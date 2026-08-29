#include <stdio.h>
#include <unistd.h>

#define CASH_PATH               "I:/bin/cash"

int main()
{
        /* arguments passed to the shell */
        const char *argv[] = {
                CASH_PATH
        };

        printf("init: executing cash...\n");

        pid_t pid = fork();
        if (pid == 0)
                exec(CASH_PATH, 1, argv, NULL);
        else
                wait();

        return 0;
}