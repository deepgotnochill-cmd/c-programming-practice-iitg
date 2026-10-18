#include <stdio.h>
#include <math.h>

float area(int a, int b, int c) {
    float s = (a + b + c) / 2.0f;
    return sqrtf(s * (s - a) * (s - b) * (s - c));
}

int perimeter(int a, int b, int c) {
    return a + b + c;
}

int main(void) {
    int a, b, c;

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