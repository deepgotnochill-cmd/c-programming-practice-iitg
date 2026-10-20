#include <stdio.h>

#define SIZE 4

int main(void)
{
    double mark[SIZE];

    printf("Addresses of array elements:\n");

    for (int i = 0; i < SIZE; i++) {
        printf("&mark[%d] = %p\n", i, (void *)&mark[i]);
    }

    printf("Array length: %d elements\n", SIZE);
    printf("Array size: %zu bytes\n", sizeof(mark));

    return 0;
}