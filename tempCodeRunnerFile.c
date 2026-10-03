#include <stdio.h>

#define cir(r) (3.14f * (r) * (r))

int main(void) {
    int radius;
    float area;

    scanf("%d", &radius);
    area = cir(radius);
    printf("Area: %f\n", area);

    return 0;
}