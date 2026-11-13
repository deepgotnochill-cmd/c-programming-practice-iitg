/*
 * Question 3: Write a function to find the position of a character in a string.
 * Prototype: int position(char *str, char c);
 * The function checks for the first occurrence of the character c in the string str
 * and prints its position. (e.g., "This is a string" and 's' -> output 4)
 */

#include <stdio.h>
#include <string.h>

int position(char *str, char c) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == c) {
            printf("Position of '%c': %d\n", c, i + 1);
            return i + 1;
        }
    }

    printf("Character '%c' not found in the string.\n", c);
    return -1;
}

int main() {
    char str[200];
    char ch;

    printf("Enter a string: ");
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = '\0';

    printf("Enter the character to find: ");
    scanf(" %c", &ch);

    position(str, ch);

    return 0;
}
