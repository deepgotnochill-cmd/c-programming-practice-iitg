#include <stdio.h>

int running_total(int n) {
    static int total = 0;
    total += n;
    return total;
}

int main(void) {
    int number;

    printf("Enter 5 numbers:\n");

    for (int i = 0; i < 5; i++) {
        if (scanf("%d", &number) != 1) {
            return 1;
        }

        printf("Running Total: %d Current number: %d\n",
               running_total(number), number);
    }

    return 0;
}