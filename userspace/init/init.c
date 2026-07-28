#include <stdio.h>
#include <unistd.h>

int main()
{
        char buf[50];

        printf("Hello from init!\n");

        int fd = open("I:/README.txt");
        read(fd, buf, sizeof(buf));
        close(fd);

        printf("I:/README.txt: %s\n", buf);

        printf("Executing shell...\n");
        pid_t pid = fork();
        if (pid == 0)
                exec("I:/bin/shell");
        else
                wait();

        return 0;
}