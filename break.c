#include <stdio.h>

int main() {
    int n;

    while (1) {
        printf("Enter a number: ");
        scanf("%d", &n);

        if (n > 0) {
            printf("I am positive\n");
            break;
        }
        else if (n < 0) {
            printf("I am negative\n");
            break;
        }
    }

    return 0;
}