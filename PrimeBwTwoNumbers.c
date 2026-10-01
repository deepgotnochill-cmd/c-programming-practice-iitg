#include <stdio.h>

int main() {
    int x, y;
    int i, j, prime;
    int found = 0;

    printf("Enter x and y: ");
    scanf("%d %d", &x, &y);

    if (x > y) {
        printf("Invalid range\n");
    }
    else {
        for (i = x; i <= y; i++) {

            if (i < 2) {
                continue;
            }

            prime = 1;

            for (j = 2; j < i; j++) {
                if (i % j == 0) {
                    prime = 0;
                    break;
                }
            }

            if (prime == 1) {
                printf("%d ", i);
                found = 1;
            }
        }

        if (found == 0) {
            printf("No prime numbers");
        }

        printf("\n");
    }

    return 0;
}