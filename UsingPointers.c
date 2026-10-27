#include <stdio.h>

int main(void) {
    int num1, num2;
    int *n1 = &num1;
    int *n2 = &num2;

    scanf("%d %d", n1, n2);
    printf("Sum is %d\n", *n1 + *n2);

    return 0;
}