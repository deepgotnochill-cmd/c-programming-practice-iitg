#include <stdio.h>

int main(void) {
    int h, m, s, total;
    scanf("%d%d%d", &h, &m, &s);

    total = h * 3600 + m * 60 + s;
    printf("Seconds: %d\n", total);
    printf("Percent of day: %.2f%%\n", total * 100.0 / 86400);

    return 0;
}