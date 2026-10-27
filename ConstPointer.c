#include <stdio.h>

int main() {
    int a = 10;
    int b = 20;

    // Constant pointer → the address stored in ptr cannot be changed
    int * const ptr = &a;

    printf("Value: %d\n", *ptr);   // 10

    *ptr = 100;                    // Allowed → changing the value is fine
    printf("Value after change: %d\n", *ptr);  // 100

    // ptr = &b;                   // ERROR! Cannot change the address

    return 0;
}