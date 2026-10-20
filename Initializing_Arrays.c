#include <stdio.h>

int main(void)
{
    double mark[250];

    for (int i = 0; i < 250; i++) {
        mark[i] = 0.0;
    }

    double values[] = {1, 2, 2, 3, 3, 4, 0};
    int count = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < count; i++) {
        mark[i] = values[i];
    }

    for (int i = 0; i < 250; i++) {
        printf("mark[%d] = %.1f\n", i, mark[i]);
    }

    return 0;
}