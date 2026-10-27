#include <stdio.h>

int main(void) {
    int num = 10;
    int *ptr1 = &num;
    int *ptr2 = NULL;
    int *ptr3 = NULL;

    if (ptr1 == NULL)
        printf("ptr1: NULL\n");
    else
        printf("ptr1: NOT NULL\n");

    if (ptr2 == NULL)
        printf("ptr2: NULL\n");
    else
        printf("ptr2: NOT NULL\n");

    if (ptr3 == NULL)
        printf("ptr3: NULL\n");
    else
        printf("ptr3: NOT NULL\n");

    return 0;
}