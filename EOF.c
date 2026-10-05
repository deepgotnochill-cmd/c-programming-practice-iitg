#include <stdio.h>

int main(void) {
    int ch;
    int count = 0;

    printf("Enter text, then signal end of input (Ctrl+D on Linux/macOS, Ctrl+Z then Enter on Windows):\n");

    while ((ch = getchar()) != EOF) {
        count++;
    }

    printf("You entered %d characters.\n", count);
    return 0;
}