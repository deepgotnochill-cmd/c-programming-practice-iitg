#include <stdio.h>
#include <stdlib.h>

void *num(int data) {
    int *result = malloc(sizeof(int));
    *result = data;
    return result;
}

int main() {
    int data = 10;
    int *ptr = (int *) num(data);
    printf("%d\n", *ptr);
    free(ptr);          // important!
    return 0;
}