#include <stdio.h>

int main(void) {
    int c;

    while ((c = getchar()) != EOF) {
        if (c >= 'a' && c <= 'z')
            putchar(c - 'a' + 'A');
        else if (c >= 'A' && c <= 'Z')
            putchar(c - 'A' + 'a');
        else
            printf("It is not an alphabet");
    }

    return 0;
}