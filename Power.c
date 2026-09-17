#include <stdio.h>

int main() {
    int i;
    
    printf("Power 1\tPower 2\tPower 3\tPower 4\tPower 5\n");
    printf("-------\t-------\t-------\t-------\t-------\n");
    
    for(i = 1; i <= 10; i++) {
        printf("%d\t%d\t%d\t%d\t%d\n", i, i*i, i*i*i, i*i*i*i, i*i*i*i*i);
    }
    
    return 0;
}