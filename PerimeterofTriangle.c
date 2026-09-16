#include <stdio.h>

int main() {
    int length, breadth;
    
    printf("Enter length and breadth: ");
    scanf("%d %d", &length, &breadth);
    
    int perimeter = 2 * (length + breadth);
    int area = length * breadth;
    
    printf("Perimeter: %d\n", perimeter);
    printf("Area: %d\n", area);
    
    return 0;
}