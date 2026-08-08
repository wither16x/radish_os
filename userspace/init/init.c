#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
        char buf[50];
        const char *shell_file = "I:/bin/shell";

        printf("Hello from init!\n");

        FILE *f = fopen("I:/README.txt");
        if (!f) {
                printf("Failed to create file object\n");
                goto exec_shell;
        }
        read(f->fd, buf, sizeof(buf));
        fclose(f);

        buf[strlen(buf) - 1] = '\0';
        printf("I:/README.txt: %s\n", buf);

exec_shell:
        printf("Executing shell...\n");
        pid_t pid = fork();
        if (pid == 0) {
                char *argv[] = {shell_file};
                exec(shell_file, 1, argv, NULL);
        } else {
                wait();
        }

        return 0;
}