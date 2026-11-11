#include <stdio.h>
#include <string.h>

int main(void) {
    char dst[30];
    char src[] = "Source";

    strcpy(dst, src);
    printf("%s\n", dst);

    return 0;
}