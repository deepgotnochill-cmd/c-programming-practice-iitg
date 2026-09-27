#include <stdio.h>

int main() {
    int i;

begin:
    for (i = 1; i <= 10; i++) {
        if (i == 5)
            goto begin;

        printf("%d", i);
    }

    return 0;
}