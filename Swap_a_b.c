#include <stdio.h>

#define SWAP(a, b) do { int t = (a); (a) = (b); (b) = t; } while (0)

int main(void) {
    int a, b;
    scanf("%d%d", &a, &b);
    SWAP(a, b);
    printf("%d %d\n", a, b);
    return 0;
}