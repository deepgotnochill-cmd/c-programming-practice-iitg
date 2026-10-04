#include <stdio.h>

int main(void) {
    double n;
    int f, c;

    scanf("%lf", &n);
    f = (int)n;
    if (n < f) f--;
    c = (n > f) ? f + 1 : f;

    printf("The floor value is %d and the ceiling is %d\n", f, c);
}