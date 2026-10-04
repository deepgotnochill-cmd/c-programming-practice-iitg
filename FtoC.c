#include <stdio.h>

#define TO_CELSIUS(f) (((f) - 32) * 5.0 / 9)

int main(void) {
    float f;
    scanf("%f", &f);
    printf("Celsius: %.2f\n", TO_CELSIUS(f));
    return 0;
}