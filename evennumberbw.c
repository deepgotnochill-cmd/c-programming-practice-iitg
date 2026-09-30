#include <stdio.h>

int main() {
    int x, y;

    printf("Enter the numbers x and y: ");
    scanf("%d %d", &x, &y);

    if (x > y) {
        printf("No even numbers possible\n");
    }
    else {
        printf("The even numbers are:");

        for (int i = x; i <= y; i++) {
            if (i % 2 == 0)
                printf("%d ", i);
        }

        printf("\n");
    }

    return 0;
}