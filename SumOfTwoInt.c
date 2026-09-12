#include <stdio.h>

int main(void) {
    int num1, num2, sum;

    scanf("%d%d", &num1, &num2);

    sum = num1 + num2;

    printf("The sum of %d and %d is %d\n", num1, num2, sum);

    return 0;
}