#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define NAME_LENGTH             50

int main(int argc, char **argv)
{
        printf("argc = %d\n", argc);
        printf("argv[0] = %s\n", argv[0]);

        char buf[NAME_LENGTH];
        printf("What is your name? ");
        fgets(buf, sizeof(buf), stdin);
        for (size_t i = 0; i < sizeof(buf); i++) {
                if (buf[i] == '\n') {
                        buf[i] = '\0';
                        break;
                }
        }
        printf("Your name is %s.\n", buf);

        return 0;
}