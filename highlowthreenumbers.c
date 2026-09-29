#include <stdio.h>

int main() {
    int a, b, c;
    int max, min;

    printf("Enter three numbers: ");
    scanf("%d %d %d", &a, &b, &c);

    max = a;
    min = a;

    if (b > max)
        max = b;

    if (c > max)
        max = c;

    if (b < min)
        min = b;

    if (c < min)
        min = c;

    printf("Maximum = %d\n", max);
    printf("Minimum = %d\n", min);

    return 0;
}