#include <stdio.h>

int main(void) {
    int number = 42;       // A regular integer
    int *pointer = &number; // A pointer stores the address of number

    printf("number's value: %d\n", number);
    printf("number's address: %p\n", (void *)&number);
    printf("pointer's value (the address it stores): %p\n", (void *)pointer);
    printf("value at that address: %d\n", *pointer);

    *pointer = 100; // Change number through the pointer
    printf("number after changing it through pointer: %d\n", number);

    return 0;
}