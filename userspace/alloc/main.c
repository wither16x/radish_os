#include <stdlib.h>
#include <stdio.h>

/// This file will be removed very soon.
/// If it seems weird to yoy, know that it used to call __debug_print_allocator_state(),
/// which was a debug function from Tomato Libc.

int main()
{
        printf("Testing tomato libc allocator...\n");

        printf("* Basic allocation:\n");
        int *arr = (int *)malloc(10 * sizeof(int));
        if (arr) {
                for (int i = 0; i < 10; i++)
                        arr[i] = i * i;

                printf("\tAllocated and filled array\n");

                free(arr);
                printf("\tFreed array\n");
        }

        printf("* Multiple allocation test:\n");
        char *s1 = (char *)malloc(100);
        char *s2 = (char *)malloc(200);
        char *s3 = (char *)malloc(50);

        printf("\tAllocated three blocks\n");

        free(s1);
        printf("\nFreed first block:\n");

        free(s3);
        printf("\nFreed third block:\n");

        free(s2);
        printf("\nFreed second block:\n");

        printf("\n* Large allocation test:\n");
        void *large_block = malloc(10000);
        printf("\tAllocated large block\n");

        free(large_block);
        printf("\tFreed large block:\n");

        printf("All allocator tests completed!\n");

        return 0;
}