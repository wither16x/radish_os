#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#define UINPUT_MAX      255

int main()
{
        char uinput[UINPUT_MAX];

        while (true) {
                printf("$ ");
                fgets(uinput, sizeof(uinput), "D:/input");

                for (size_t i = 0; i < sizeof(uinput); i++) {
                        if (uinput[i] == '\n') {
                                uinput[i] = '\0';
                                break;
                        }
                }

                pid_t pid = fork();
                if (pid == 0) {
                        exec(uinput);
                        return 1; // failed to launch program
                } else {
                        int wait_res = wait();
                        if (wait_res == -1) {
                                printf("there is no current process\n");
                        } else if (wait_res == -2) {
                                printf("current process has no child\n");
                        } else if (wait_res == -3) {
                                printf("no dead child found\n");
                        }
                }
        }

        return 0;
}