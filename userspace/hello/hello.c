#include <unistd.h>

int main()
{
        write("D:/console", "Hello from syscall write!\r\n", 27);

        return 0;
}