#include <stdio.h>

int main() {
    FILE *ifp;
    int c; // Use 'int' instead of 'char' to properly handle EOF

    ifp = fopen("Test.txt", "r");
    if (ifp == NULL) {
        printf("Error: Could not open Test.txt\n");
        return 1;
    }

    do {
        c = fgetc(ifp);
        if (feof(ifp)) {
            break;
        }
        printf("%c", c);
    } while (1);

    fclose(ifp);
    return 0;
}
