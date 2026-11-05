#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *p = malloc(10 * sizeof *p);

    if (p == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        p[i] = (i + 1) * 10;
    }

    for (int i = 0; i < 10; i++) {
        printf("p[%d] = %d\n", i, p[i]);
    }

    free(p);
    return 0;
}