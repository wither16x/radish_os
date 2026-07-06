#include <unistd.h>

int main()
{
        char buf;
        write("D:/console", "Type some text: ", 16);

        while (true) {
                read("D:/input", &buf, sizeof(buf));
                if (buf)
                        write("D:/console", &buf, sizeof(buf));
        }

        return 0;
}