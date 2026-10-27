#include <stdio.h>

int main() {
    char cvar, *cptr = &cvar;
    int ivar, *iptr = &ivar;
    float fvar, *fptr = &fvar;
    double dvar, *dptr = &dvar;
    long double ldvar, *ldptr = &ldvar;

    printf("Size of char pointer %lu\n", sizeof(cptr));
    printf("Size of int pointer %lu\n", sizeof(iptr));
    printf("Size of float pointer %lu\n", sizeof(fptr));
    printf("Size of double pointer %lu\n", sizeof(dptr));
    printf("Size of long double pointer %lu\n", sizeof(ldptr));

    return 0;
}