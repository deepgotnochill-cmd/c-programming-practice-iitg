#include <stdio.h>

// Call by Value → only copies are swapped
void swap(int a, int b) {
    int temp = a;
    a = b;
    b = temp;

    printf("Inside swap: a = %d, b = %d\n", a, b);
}

int main() {
    int x = 10, y = 20;

    printf("Before swap: x = %d, y = %d\n", x, y);

    swap(x, y);   // Passing values (copies)

    printf("After swap:  x = %d, y = %d\n", x, y);

    return 0;
}