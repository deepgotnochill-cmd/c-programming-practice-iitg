#include <stdio.h>

#define sq(x) ((x) * (x))

int main(void) {
    int x = 25 / sq(5);
    printf("%d\n", x);
    return 0;
}