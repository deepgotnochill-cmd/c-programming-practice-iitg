#include <stdio.h>

int main() {
    int n, num;
    int largest, smallest;

    printf("Enter the value of n: ");
    scanf("%d", &n);

    printf("Enter the %d numbers:\n", n);

    scanf("%d", &num);
    largest = smallest = num;

    for (int i = 2; i <= n; i++) {
        scanf("%d", &num);

        if (num > largest)
            largest = num;

        if (num < smallest)
            smallest = num;
    }

    printf("The largest number is %d and the smallest number is %d.\n",
           largest, smallest);

    return 0;
}