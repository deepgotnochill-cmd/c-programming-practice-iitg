/*
 * Question 4: A string is said to be a palindrome if the reverse of the string reads
 * the same as the original string, for example, "madam". Write a function using
 * pointers to check if a string is a palindrome.
 * Prototype: void is_palindrome(char *str);
 */

#include <stdio.h>
#include <string.h>

void is_palindrome(char *str) {
    char *start = str;
    char *end = str + strlen(str) - 1;

    while (start < end) {
        if (*start != *end) {
            printf("\"%s\" is NOT a palindrome.\n", str);
            return;
        }
        start++;
        end--;
    }

    printf("\"%s\" IS a palindrome.\n", str);
}

int main() {
    char str[200];

    printf("Enter a string: ");
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = '\0';

    is_palindrome(str);

    return 0;
}
