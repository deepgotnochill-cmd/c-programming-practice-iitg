#include <stdio.h>

int sum(int value);

int main(void) {
    int value;

    scanf("%d", &value);
    while (value != EOF) {
        printf("Running sum: %d\n", sum(value));
        scanf("%d", &value);
    }

    return 0;
}

int sum(int value) {
    static int total = 0;
    total += value;
    return total;
}