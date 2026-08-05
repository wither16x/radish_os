#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
        char buf[50];

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
                char *argv[] = {"I:/bin/shell"};
                exec("I:/bin/shell", 1, argv, NULL);
        } else {
                wait();
        }

        return 0;
}