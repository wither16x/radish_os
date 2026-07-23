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

                int pid = fork();
                printf("PID = %d\n", pid);
                if (pid == 0) {
                        printf("executing...\n");
                        exec(uinput);
                        return 1; // failed to launch program
                } else {
                        printf("waiting...\n");
                        int wait_res = wait();
                        if (wait_res == -1) {
                                printf("there is no current process\n");
                                return 1;
                        } else if (wait_res == -2) {
                                printf("current process has no child\n");
                                return 1;
                        } else if (wait_res == -3) {
                                printf("no dead child found\n");
                        }
                }
        }

        return 0;
}