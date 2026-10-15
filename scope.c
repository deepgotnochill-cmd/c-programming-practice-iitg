#include <stdio.h>

int multiply(int, int);  // Function declaration

int main(void) {
    int a = 2;
    int b = 5;
    int c = multiply(a, b);

    printf("Inside main: a = %d, b = %d, c = %d\n", a, b, c);

    a = multiply(b, c);
    printf("After second call: a = %d\n", a);

    return 0;
}

int multiply(int x, int y) {
    int result = x * y;  // x, y, and result are local to multiply
    printf("Inside multiply: x = %d, y = %d, result = %d\n",
           x, y, result);

    return result;
}