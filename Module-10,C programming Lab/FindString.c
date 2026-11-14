#include <stdio.h>
#include <string.h>

int main() {
    char str[100], ch;
    int i, found = 0;

    // Take string input (including spaces)
    printf("Enter the string: ");
    fgets(str, sizeof(str), stdin);
    
    // Remove trailing newline character added by fgets
    str[strcspn(str, "\n")] = '\0';

    // Take character input
    printf("Enter the character: ");
    scanf(" %c", &ch);

    // Iterate through the string
    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] == ch) {
            printf("Index of %c is %d\n", ch, i);
            found = 1;
            break; // Stop at the first occurrence
        }
    }

    // If character was not found
    if (!found) {
        printf("Character %c is not present\n", ch);
    }

    return 0;
}
