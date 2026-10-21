#include <stdio.h>

int main(void) {
    double values[] = {1.1, 2.2, 3.3, 4.4, 5.5};
    int length = sizeof(values) / sizeof(values[0]);

    printf("Array elements and addresses:\n");

    for (int i = 0; i < length; i++) {
        printf("values[%d] = %.1f, address = %p\n",
               i, values[i], (void *)&values[i]);
    }

    printf("Array length: %d\n", length);
    return 0;
}