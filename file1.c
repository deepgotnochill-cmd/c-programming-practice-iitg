#include <stdio.h>

extern int a, b, c;

float area(int, int, int);
int perimeter(int, int, int);

int main(void) {
    printf("Enter the sides of the triangle: ");
    if (scanf("%d %d %d", &a, &b, &c) != 3) {
        return 1;
    }

    if (a <= 0 || b <= 0 || c <= 0 ||
        a + b <= c || a + c <= b || b + c <= a) {
        printf("Not valid input.\n");
        return 0;
    }

    printf("Perimeter = %d\n", perimeter(a, b, c));
    printf("Area = %f\n", area(a, b, c));

    return 0;
}