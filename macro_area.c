#include <stdio.h>

#define CIRCLE(r) (3.14 * (r) * (r))
#define SQUARE(s) ((s) * (s))
#define RECTANGLE(l, w) ((l) * (w))

int main(void) {
    printf("Circle area: %.2f\n", CIRCLE(5));
    printf("Square area: %d\n", SQUARE(4));
    printf("Rectangle area: %d\n", RECTANGLE(4, 6));
    return 0;
}