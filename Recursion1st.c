#include <stdio.h>

int factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

int main(void) {
    int n;

    printf("Enter a nonnegative integer: ");
    if (scanf("%d", &n) != 1 || n < 0) {
        printf("Please enter a nonnegative integer.\n");
        return 1;
    }

    printf("%d! = %d\n", n, factorial(n));
    return 0;
}