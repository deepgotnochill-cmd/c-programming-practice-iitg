#include <stdio.h>

#define MAX_SIZE 100

int main(void)
{
    int num[MAX_SIZE];
    int n;

    printf("How many positive numbers? ");
    if (scanf("%d", &n) != 1 || n < 3 || n > MAX_SIZE) {
        printf("Please enter a count from 3 to %d.\n", MAX_SIZE);
        return 1;
    }

    printf("Enter %d positive numbers:\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &num[i]) != 1 || num[i] <= 0) {
            printf("Please enter positive integers only.\n");
            return 1;
        }
    }

    int largest = 0;
    int second = 0;
    int third = 0;

    for (int i = 0; i < n; i++) {
        if (num[i] > largest) {
            third = second;
            second = largest;
            largest = num[i];
        } else if (num[i] != largest && num[i] > second) {
            third = second;
            second = num[i];
        } else if (num[i] != largest && num[i] != second && num[i] > third) {
            third = num[i];
        }
    }

    if (third == 0) {
        printf("There are fewer than three distinct values.\n");
        return 1;
    }

    printf("Three largest distinct values: %d, %d, %d\n",
           largest, second, third);

    return 0;
}