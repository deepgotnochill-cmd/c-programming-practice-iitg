#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int n;
    float *p;

    printf("How many numbers? ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Please enter a positive integer.\n");
        return 1;
    }

    p = malloc((size_t)n * sizeof *p);
    if (p == NULL) {
        printf("Allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("Enter number %d: ", i + 1);
        if (scanf("%f", &p[i]) != 1) {
            printf("Invalid number.\n");
            free(p);
            return 1;
        }
    }

    printf("You entered:\n");
    for (int i = 0; i < n; i++) {
        printf("%.2f\n", p[i]);
    }

    free(p);
    return 0;
}