#include <stdio.h>

int main(void) {
    int a = 1, b = 2, c = 3;
    printf("Block 1: a=%d, b=%d, c=%d\n", a, b, c);

    {
        int b = 4;
        float c = 5.0f;
        printf("Block 2: a=%d, b=%d, c=%.1f\n", a, b, c);

        a = b;

        {
            int c = b;
            printf("Block 3: a=%d, b=%d, c=%d\n", a, b, c);
        }

        printf("Back in block 2: a=%d, b=%d, c=%.1f\n", a, b, c);
    }

    printf("Back in block 1: a=%d, b=%d, c=%d\n", a, b, c);

    return 0;
}