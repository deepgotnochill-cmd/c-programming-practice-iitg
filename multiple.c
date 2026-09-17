#include <stdio.h>

int main() {
    int dividend, divisor1, divisor2;
    
    scanf("%d %d %d", &dividend, &divisor1, &divisor2);
    
    if(dividend % divisor1 == 0 && dividend % divisor2 == 0) {
        printf("1\n");
    } else {
        printf("0\n");
    }
    
    return 0;
}