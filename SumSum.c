#include <stdio.h>

int SumSeries(int a, int n) {
    int sum = 1;
    int term = 1;

    for (int i = 1; i <= n; i++) {
        term *= a;
        sum += term;
    }

    return sum;
}

int main(void) {
    int a, n;
    scanf("%d %d", &a, &n);
    printf("%d\n", SumSeries(a, n));
    return 0;
}