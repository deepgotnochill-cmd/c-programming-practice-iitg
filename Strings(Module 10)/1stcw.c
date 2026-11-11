#include <stdio.h>
#include <ctype.h>

void count_words(const char *p) {
    int words = 0;
    int characters = 0;
    int in_word = 0;

    while (*p != '\0') {
        unsigned char ch = (unsigned char)*p;

        if (isspace(ch)) {
            in_word = 0;
        } else {
            ++characters;
            if (!in_word) {
                ++words;
                in_word = 1;
            }
        }

        ++p;
    }

    printf("Words = %d\tCharacters = %d\n", words, characters);
}

int main(void) {
    char text[256];

    printf("Enter a line: ");
    if (fgets(text, sizeof text, stdin) != NULL) {
        count_words(text);
    }

    return 0;
}