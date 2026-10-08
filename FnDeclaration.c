#include <stdio.h>

int add(int x, int y) {
    return x + y;
}

int main(void) {
    int a, b;

    scanf("%d", &a);
    scanf("%d", &b);

    printf("%d\n", add(a, b));
    return 0;
}