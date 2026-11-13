/*
 * Question 1: Write a function to count the number of characters and words in a string.
 * The program should also consider the case when the user may input no word.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

void countCharsAndWords(char *str) {
    int charCount = 0;
    int wordCount = 0;
    int inWord = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        charCount++;

        if (isspace((unsigned char)str[i])) {
            inWord = 0;
        } else if (inWord == 0) {
            inWord = 1;
            wordCount++;
        }
    }

    printf("Number of characters: %d\n", charCount);
    printf("Number of words: %d\n", wordCount);
}

int main() {
    char str[200];

    printf("Enter a string: ");
    fgets(str, sizeof(str), stdin);

    // Remove trailing newline character added by fgets
    str[strcspn(str, "\n")] = '\0';

    countCharsAndWords(str);

    return 0;
}
