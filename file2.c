#include <math.h>

int a, b, c;

float area(int x, int y, int z) {
    float s = (x + y + z) / 2.0f;
    return sqrtf(s * (s - x) * (s - y) * (s - z));
}