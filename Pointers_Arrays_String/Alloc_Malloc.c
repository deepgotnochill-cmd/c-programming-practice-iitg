#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int count = 5;

    int *a = malloc(count * sizeof *a);
    if (a == NULL) {
        printf("malloc failed\n");
        return 1;
    }

    int *b = calloc(count, sizeof *b);
    if (b == NULL) {
        printf("calloc failed\n");
        free(a);
        return 1;
    }

    /* Initialize malloc memory before reading it. */
    for (int i = 0; i < count; i++) {
        a[i] = (i + 1) * 10;
    }

    printf("malloc values (assigned by the program): ");
    for (int i = 0; i < count; i++) {
        printf("%d ", a[i]);
    }

    printf("\ncalloc values (zero-initialized): ");
    for (int i = 0; i < count; i++) {
        printf("%d ", b[i]);
    }

    printf("\n");

    free(a);
    free(b);
    return 0;
}