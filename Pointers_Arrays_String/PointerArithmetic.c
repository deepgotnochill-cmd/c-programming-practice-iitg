#include <stdio.h>

int main(void)
{
    int numbers[] = {10, 20, 30, 40, 50};
    int *ptr = numbers;
    size_t count = sizeof numbers / sizeof numbers[0];

    printf("Elements using pointer arithmetic:\n");
    for (size_t i = 0; i < count; ++i) {
        printf("*(ptr + %zu) = %d\n", i, *(ptr + i));
    }

    printf("\nMoving the pointer one element at a time:\n");
    for (size_t i = 0; i < count; ++i) {
        printf("Address: %p  Value: %d\n", (void *)ptr, *ptr);
        ++ptr;
    }

    return 0;
}