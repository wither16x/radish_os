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

                exec(uinput);
        }

        return 0;
}