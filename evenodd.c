#include <stdio.h>

int main() {
    int n, num;

    printf("Enter the number n: ");
    scanf("%d", &n);

    for (int i = 1; i <= n; i++) {
        scanf("%d", &num);

        if (num % 2 == 0)
            printf("The number is even\n");
        else
            printf("The number is odd\n");
    }

    return 0;
}