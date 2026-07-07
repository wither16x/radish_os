#include <stdio.h>
#include <unistd.h>

int main()
{
        const char *name = "Wither__";
        int age = 15;
        printf("Hello! My name is %s and I am %d years old!\r\n", name, age);

        char buf;
        printf("Type some text: ");

        while (true) {
                read("D:/input", &buf, sizeof(buf));
                if (buf)
                        putchar(buf);
        }

        return 0;
}