#include <stdio.h>
#include <string.h>

int main() {
    char s1[] = "beautiful big sky country";
    char s2[] = "how now brown cow";

    // 1. strlen(s1)
    printf("strlen(s1): %zu\n", strlen(s1));

    // 2. strlen(s2 + 8)
    printf("strlen(s2 + 8): %zu\n", strlen(s2 + 8));

    // 3. strcmp(s1, s2)
    printf("strcmp(s1, s2): %d\n", strcmp(s1, s2));

    // 4. printf("%s", s1 + 10)
    printf("printf(\"%%s\", s1 + 10): ");
    printf("%s", s1 + 10);
    printf("\n");

    // 5. strcpy(s1+10, s2+8)
    strcpy(s1 + 10, s2 + 8);
    printf("After strcpy, s1 is: \"%s\"\n", s1);

    // 6. strcat(s2, "!")
    strcat(s2, "!");
    printf("After strcat, s2 is: \"%s\"\n", s2);

    return 0;
}
