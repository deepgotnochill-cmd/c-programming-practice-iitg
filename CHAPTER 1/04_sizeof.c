
#include <stdio.h>
#include <limits.h>

int main() {
    printf("Size of int = %zu bits\n", sizeof(int) * CHAR_BIT);
    printf("Size of float = %zu bits\n", sizeof(float) * CHAR_BIT);
    printf("Size of char = %zu bits\n", sizeof(char) * CHAR_BIT);

    return 0;
}
