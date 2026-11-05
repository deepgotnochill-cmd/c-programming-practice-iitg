#include <stdio.h>

int main(void)
{
    int numbers[] = {10, 20, 30, 40, 50};
    int *ptr = numbers;
    size_t count = sizeof numbers / sizeof numbers[0];

    for (size_t i = 0; i < count; ++i) {
        printf("numbers[%zu] = %d, *(ptr + %zu) = %d\n",
               i, numbers[i], i, *(ptr + i));
    }

    return 0;
}