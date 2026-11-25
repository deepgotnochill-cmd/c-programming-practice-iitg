#include <stdio.h>
#include <math.h>

#define PI 3.14159

enum ShapeType {
    CIRCLE = 0, RECTANGLE = 1, TRIANGLE = 2
};

struct Shape {
    enum ShapeType type;
    union {
        struct { float radius; } circle;
        struct { float length, breadth; } rectangle;
        struct { float a, b, c; } triangle;
    } dimensions;
};

int main(void) {
    struct Shape s;
    int choice;
    float area = 0;

    printf("Which shape do you want to store?\n");
    printf("Circle(0), Rectangle(1), Triangle(2): ");
    scanf("%d", &choice);

    if (choice < 0 || choice > 2) {
        printf("Invalid choice!\n");
        return 1;
    }
    s.type = (enum ShapeType)choice;

    switch (s.type) {
        case CIRCLE:
            printf("Enter radius: ");
            scanf("%f", &s.dimensions.circle.radius);
            if (s.dimensions.circle.radius <= 0) {
                printf("Invalid radius!\n");
                return 1;
            }
            area = PI * s.dimensions.circle.radius
                      * s.dimensions.circle.radius;
            break;
        case RECTANGLE:
            printf("Enter length and breadth: ");
            scanf("%f %f", &s.dimensions.rectangle.length,
                            &s.dimensions.rectangle.breadth);
            if (s.dimensions.rectangle.length <= 0 ||
                s.dimensions.rectangle.breadth <= 0) {
                printf("Invalid dimensions!\n");
                return 1;
            }
            area = s.dimensions.rectangle.length
                 * s.dimensions.rectangle.breadth;
            break;
        case TRIANGLE: {
            float a, b, c, semi;
            printf("Enter three sides: ");
            scanf("%f %f %f", &a, &b, &c);
            if (a <= 0 || b <= 0 || c <= 0 ||
                a + b <= c || a + c <= b || b + c <= a) {
                printf("Invalid triangle!\n");
                return 1;
            }
            s.dimensions.triangle.a = a;
            s.dimensions.triangle.b = b;
            s.dimensions.triangle.c = c;
            semi = (a + b + c) / 2;
            area = sqrtf(semi * (semi - a)
                             * (semi - b) * (semi - c));
            break;
        }
    }

    printf("Area is %.2f\n", area);
    return 0;
}
