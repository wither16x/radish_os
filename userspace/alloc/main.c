#include <stdlib.h>
#include <stdio.h>

int main()
{
        printf("Testing tomato libc allocator...\n");

        printf("* Basic allocation:\n");
        int *arr = (int *)malloc(10 * sizeof(int));
        if (arr) {
                for (int i = 0; i < 10; i++)
                        arr[i] = i * i;

                printf("\tAllocated and filled array\n");
                __debug_print_allocator_state();

                free(arr);
                printf("\tFreed array\n");
                __debug_print_allocator_state();
        }

        return 0;
}