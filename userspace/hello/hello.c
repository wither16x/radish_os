#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

int main()
{
        printf("Type some text: ");

        while (true) {
                int ch = getchar();
                putchar(ch);
        }
        
        return 0;
}